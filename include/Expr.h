//
// Created by aLIEzTed on 10/29/16.
//

#ifndef C_PARSER_EXPR_H
#define C_PARSER_EXPR_H

#include "Token.h"
#include "AST.h"
#include <vector>

namespace cParser {
class Expr {
 private:
  std::vector<Token *> mTokens;

  Token *curToken;
  int pos = 0;
  Token *&getNextToken() {
    return curToken = mTokens[pos++];
  }

  Token *&lookAheadToken(int offset) {
    return mTokens[pos + offset - 1];
  }

 public:
  std::vector<cParser::Statement *> statements;
  Expr(const vector<Token *> &tokens) : mTokens(tokens) { }

  bool parse();

  cParser::Statement *parseIfExpr();

  cParser::Statement *parseStatement();


  cParser::Statement *parseDeclare();

  cParser::Statement *parseSwitchExpr();

  cParser::Statement *parseForExpr();

  cParser::Statement *parseWhileExpr();

  cParser::Statement *parseExpr();

  cParser::Statement *parseVarDeclaration();

  cParser::Statement *parseFuncDeclaration();

  cParser::Statement* parseDowhileExpr();
};
}
#endif //C_PARSER_EXPR_H
