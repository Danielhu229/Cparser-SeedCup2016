//
// Created by 胡一鸣 on 16/10/25.
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

struct Statement {
  ASTType type;
  Token token;
  vector<Statement*> children;
  Statement(ASTType type, Token token) : type(type), token(token), children({}) {}
  virtual ~Statement() {}
};
}

#endif // C_PARSER_AST_H
