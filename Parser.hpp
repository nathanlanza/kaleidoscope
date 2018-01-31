#ifndef Parser_hpp
#define Parser_hpp

#include <memory>
#include "AST.hpp"

extern std::map<char, int> BinopPrecedence;

void InitializeBinops();

std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<ExprAST> ParseNumberExpr();
std::unique_ptr<ExprAST> ParseParenExpr();
std::unique_ptr<ExprAST> ParseIdentifierExpr();
std::unique_ptr<ExprAST> ParseIfExpr();
std::unique_ptr<ExprAST> ParseForExpr();
std::unique_ptr<ExprAST> ParsePrimary();
std::unique_ptr<ExprAST> ParseUnary();
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<PrototypeAST> ParsePrototype();
std::unique_ptr<FunctionAST> ParseDefinition();
std::unique_ptr<FunctionAST> ParseTopLevelExpr();
std::unique_ptr<PrototypeAST> ParseExtern();

void HandleTopLevelExpression();
void HandleExtern();
void HandleDefinition();
void MainLoop();

#endif /* Parser_hpp */
