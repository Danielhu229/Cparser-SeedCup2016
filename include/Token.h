//
// Created by aLIEzTed on 10/25/16.
//

#ifndef SEEDCUP_TOKEN_H
#define SEEDCUP_TOKEN_H
#include "token_type.h"
#include <string>
#include <ostream>
#include <vector>
#include "token_type.h"
class Token {
 public:
  token_type token;
  // value of this token, used in var, char, string literal
  std::string token_val;
  std::string name;
  int type;
  int clz;
  Token(std::string name, token_type symbol);
  Token();
  Token(const Token& obj);
  Token(std::string name, token_type symbol, std::string token_val);
  bool operator==(const Token &rhs) const;
  bool operator!=(const Token &rhs) const;
  bool operator<(const Token &rhs) const;
  bool operator>(const Token &rhs) const;
  bool operator<=(const Token &rhs) const;
  bool operator>=(const Token &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const Token &token);
};


#endif //SEEDCUP_TOKEN_H
