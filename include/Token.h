//
// Created by aLIEzTed on 10/25/16.
//

#ifndef SEEDCUP_TOKEN_H
#define SEEDCUP_TOKEN_H
#include "TokenType.h"
#include <string>
#include <ostream>
#include <vector>

using namespace std;

namespace cParser {

class Token {
 public:
  TokenType type;
  // value of this token, used in var, char, string literal
  std::string str;
  Token();
  Token(const Token &obj);
  Token(string, TokenType);
  bool operator==(const Token &rhs) const;
  bool operator!=(const Token &rhs) const;
  bool operator<(const Token &rhs) const;
  bool operator>(const Token &rhs) const;
  bool operator<=(const Token &rhs) const;
  bool operator>=(const Token &rhs) const;
};

}

#endif //SEEDCUP_TOKEN_H
