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
  int pos;
  Token *&getNextToken() {
    return curToken = mTokens[pos++];
  }

  Token *&lookAheadToken(int offset) {
    return mTokens[pos + offset - 1];
  }

  /*!
   * parse the if statement.
   * @return an if statement.
   */
  cParser::Statement *parseIfExpr();

  /*!
   * Dispatch method to a statement
   * @return a target statement.
   */
  cParser::Statement *parseStatement();

  /*!
   * parse the declare statement
   * @return a declaration statement
   */
  cParser::Statement *parseDeclare();

  /*!
   * parse a switch statement
   * @return a switch statement
   */
  cParser::Statement *parseSwitchExpr();

  /*!
   * parse a for statement
   * @return a for statement
   */
  cParser::Statement *parseForExpr();

  /*!
   * parse a while statement
   * @return a while statement
   */
  cParser::Statement *parseWhileExpr();

  /*!
   * parse a expression
   * @return a expression statement
   */
  cParser::Statement *parseExpr();

  /*!
   * parse a declaration statement of variable
   * @return a declaration statement of variable
   */
  cParser::Statement *parseVarDeclaration();

  /*!
   * parse a declaration statement of function
   * @return a declaration statement of function
   */
  cParser::Statement *parseFuncDeclaration();

  /*!
   * parse a dowhile statement
   * @return a dowhile statement
   */
  cParser::Statement* parseDowhileExpr();

 public:
  std::vector<cParser::Statement *> statements;
  Expr(const vector<Token *> &tokens) : mTokens(tokens), pos(0){ }

  /*!
   * the entrance of parsing
   * @return true if parsing successly, false otherwise.
   */
  bool parse();


};
}
#endif //C_PARSER_EXPR_H
