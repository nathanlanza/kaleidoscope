#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "/Users/lanza/Projects/llvm/examples/Kaleidoscope/include/KaleidoscopeJIT.h"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Utilities.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "CodeGen.hpp"
#include "Parser.hpp"


//===---------------------------------------------------------
// "Library" functions that can be "extern'd" from user code.
//===---------------------------------------------------------

#ifdef LLVM_ON_WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT double putchard(double X) {
  putc((char)X, stderr);
  return 0;
}

extern "C" DLLEXPORT double printd(double X) {
  fprintf(stderr, "%f\n", X);
  return 0;
}


int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  InitializeBinops(); 

  fprintf(stderr, "ready> ");
  getNextToken();

  InitializeJIT();
  InitializeModuleAndPassManager();

  MainLoop();

  return 0;
}
































