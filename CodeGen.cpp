#include <memory>

#include "Utilities.hpp"
#include "CodeGen.hpp"
#include "AST.hpp"
#include "Parser.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "/Users/lanza/Projects/llvm/examples/Kaleidoscope/include/KaleidoscopeJIT.h"


//===-----------------------------------------------------
// Code Generation
//===-----------------------------------------------------

llvm::LLVMContext TheContext;
llvm::IRBuilder<> Builder(TheContext);
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value *> NamedValues;
std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

void InitializeJIT() {
  TheJIT = llvm::make_unique<llvm::orc::KaleidoscopeJIT>();
}

llvm::Function *getFunction(std::string Name) {
  if (auto *F = TheModule->getFunction(Name))
    return F;

  auto FI = FunctionProtos.find(Name);
  if (FI != FunctionProtos.end())
    return FI->second->codegen();

  return nullptr;
}

llvm::Value *UnaryExprAST::codegen() {
  llvm::Value *OperandV = Operand->codegen();
  if (!OperandV)
    return nullptr;

  llvm::Function *F = getFunction(std::string("unary") + Opcode);
  if (!F)
    return LogErrorV("Unknown unary operator");

  return Builder.CreateCall(F, OperandV, "unop");
}

llvm::Value *NumberExprAST::codegen() {
  return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}

llvm::Value *BinaryExprAST::codegen() {
  llvm::Value *L = LHS->codegen();
  llvm::Value *R = RHS->codegen();

  if (!L || !R)
    return nullptr;

  switch (Op) {
    case '+':
      return Builder.CreateFAdd(L, R, "addtmp");
    case '-':
      return Builder.CreateFSub(L, R, "subtmp");
    case '*':
      return Builder.CreateFMul(L, R, "multmp");
    case '<':
      L = Builder.CreateFCmpULT(L, R, "cmptmp");
      return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    default:
      break;
  }

  llvm::Function *F = getFunction(std::string("binary") + Op);
  assert(F && "binary operator not found!");

  llvm::Value *Ops[2] = { L, R };
  return Builder.CreateCall(F, Ops, "binop");
}

llvm::Value *VariableExprAST::codegen() {
  llvm::Value *V = NamedValues[Name];
  if (!V)
    LogErrorV("Unknown variable name");
  return V;
}

llvm::Value *IfExprAST::codegen() {
  llvm::Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  CondV = Builder.CreateFCmpONE(CondV, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "ifcond");
  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

  llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
  llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(TheContext, "else");
  llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  Builder.SetInsertPoint(ThenBB);
  llvm::Value *ThenV = Then->codegen();
  if (!ThenV)
    return nullptr;

  Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);

  llvm::Value *ElseV = Else->codegen();
  if (!ElseV)
    return nullptr;

  Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  llvm::PHINode *PN = Builder.CreatePHI(llvm::Type::getDoubleTy(TheContext), 2, "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
}

llvm::Value *ForExprAST::codegen() {
  llvm::Value *StartVal = Start->codegen();
  if (!StartVal)
    return nullptr;

  llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
  llvm::BasicBlock *PreheaderBB = Builder.GetInsertBlock();
  llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "loop", TheFunction);

  Builder.CreateBr(LoopBB);

  Builder.SetInsertPoint(LoopBB);

  llvm::PHINode *Variable = Builder.CreatePHI(llvm::Type::getDoubleTy(TheContext), 2, VarName.c_str());
  Variable->addIncoming(StartVal, PreheaderBB);

  llvm::Value *OldVal = NamedValues[VarName];
  NamedValues[VarName] = Variable;

  if (!Body->codegen())
    return nullptr;

  llvm::Value *StepVal = nullptr;
  if (Step) {
    StepVal = Step->codegen();
    if (!StepVal)
      return nullptr;
  } else {
    StepVal = llvm::ConstantFP::get(TheContext, llvm::APFloat(1.0));
  }

  llvm::Value *NextVar = Builder.CreateFAdd(Variable, StepVal, "nextvar");

  llvm::Value *EndCond = End->codegen();
  if (!EndCond)
    return nullptr;

  EndCond = Builder.CreateFCmpONE(EndCond, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "loopcond");

  llvm::BasicBlock *LoopEndBB = Builder.GetInsertBlock();
  llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "afterloop", TheFunction);

  Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
  Builder.SetInsertPoint(AfterBB);

  Variable->addIncoming(NextVar, LoopEndBB);

  if (OldVal)
    NamedValues[VarName] = OldVal;
  else
    NamedValues.erase(VarName);

  return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(TheContext));
}

llvm::Value *CallExprAST::codegen() {
  llvm::Function *CalleeF = getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

llvm::Function *PrototypeAST::codegen() {
  std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(TheContext));
  llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), Doubles, false);
  llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

llvm::Function *FunctionAST::codegen() {
  auto &P = *Proto;
  FunctionProtos[Proto->getName()] = std::move(Proto);
  llvm::Function *TheFunction = getFunction(P.getName());

  if (!TheFunction)
    return nullptr;

  if (P.isBinaryOp())
    BinopPrecedence[P.getOperatorName()] = P.getBinaryPrecedence();

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[Arg.getName()] = &Arg;

  if (llvm::Value *RetVal = Body->codegen()) {
    Builder.CreateRet(RetVal);

    llvm::verifyFunction(*TheFunction);

    TheFPM->run(*TheFunction);

    return TheFunction;
  }
  TheFunction->eraseFromParent();
  return nullptr;
}

void InitializeModuleAndPassManager() {
  TheModule = llvm::make_unique<llvm::Module>("my cool jit", TheContext);
  TheModule->setDataLayout(TheJIT->getTargetMachine().createDataLayout());

  TheFPM = llvm::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

  TheFPM->add(llvm::createInstructionCombiningPass());
  TheFPM->add(llvm::createReassociatePass());
  TheFPM->add(llvm::createGVNPass());
  TheFPM->add(llvm::createCFGSimplificationPass());

  TheFPM->doInitialization();
}
