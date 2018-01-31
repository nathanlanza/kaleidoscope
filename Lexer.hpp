#ifndef Lexer_hpp
#define Lexer_hpp

extern std::string IdentifierStr;
extern double NumVal;
extern int CurTok;
extern int getNextToken();

enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // if/else
  tok_if = -6,
  tok_then = -7,
  tok_else = -8,

  // for
  tok_for = -9,
  tok_in = -10,

  // operators
  tok_binary = -11,
  tok_unary = -12
};

#endif /* scanner_hpp */
