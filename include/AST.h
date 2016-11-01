//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/25.
//

#ifndef C_PARSER_AST_H
#define C_PARSER_AST_H

#include "ASTType.h"
#include "Token.h"
#include <cassert>
#include <memory>
#include <vector>

using namespace std;

namespace cParser {
/**
 * the abstract syntax tree class
 */
struct Statement {
  ASTType type;
  Token token;
  /**
   * childrens of this AST node
   */
  vector<Statement *> children;
  Statement(ASTType type, Token token)
      : type(type), token(token), children({}) {}
};
}

#endif // C_PARSER_AST_H
