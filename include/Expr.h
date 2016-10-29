//
// Created by aLIEzTed on 10/29/16.
//

#ifndef C_PARSER_EXPR_H
#define C_PARSER_EXPR_H

#include "Token.h"
#include "AST.h"
#include <vector>

using namespace cParser;
class Expr {
 private:
  std::vector<Token *> mTokens;

  Token *curToken;
  int pos = 0;
  Token* &getNextToken() {
    return curToken = mTokens[pos++];
  }

  Token* &lookAheadToken(int offset) {
    return mTokens[pos+offset-1];
  }

 public:
  std::vector<shared_ptr<cParser::Statement>> statements;
  Expr(const vector<Token *> &tokens) : mTokens(tokens) {}

  bool parse();

  shared_ptr<cParser::Statement> parseIfExpr();

  shared_ptr<cParser::Statement> parseStatement();

  shared_ptr<cParser::Statement> parseBlock();

  shared_ptr<cParser::Statement> parseSColon();

  shared_ptr<cParser::Statement> parseDeclare();

  shared_ptr<cParser::Statement> parseSwitchExpr();

  shared_ptr<cParser::Statement> parseBinary();

  shared_ptr<cParser::Statement> parseSelf();

  shared_ptr<cParser::Statement> parseForExpr();

  shared_ptr<cParser::Statement> parseWhileExpr();

  shared_ptr<cParser::Statement> parseExpr();

  shared_ptr<Statement> parseVarDeclaration();

  shared_ptr<Statement> parseFuncDeclaration();

};

#endif //C_PARSER_EXPR_H
