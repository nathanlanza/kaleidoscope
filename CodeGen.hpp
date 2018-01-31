#ifndef CodeGen_hpp
#define CodeGen_hpp

#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "/Users/lanza/Projects/llvm/examples/Kaleidoscope/include/KaleidoscopeJIT.h"

void InitializeJIT();
void InitializeModuleAndPassManager();

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value *> NamedValues;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

#endif /* CodeGen_hpp */
