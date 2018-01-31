#ifndef Utilities_hpp
#define Utilities_hpp

#include "AST.hpp"
#include <memory>

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
llvm::Value *LogErrorV(const char *Str);

#endif /* Utilities_hpp */
