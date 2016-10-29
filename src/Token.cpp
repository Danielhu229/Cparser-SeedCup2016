//
// Created by aLIEzTed on 10/25/16.
//

#include "Token.h"

using namespace cParser;

Token::Token(std::string str, TokenType type)
    :str(str), type(type) {
}

Token::Token(const Token &obj)
    :str(obj.str), type(obj.type) {
}

Token::Token():type(TokenType::S_Colon), str("") {
}

bool Token::operator==(const Token &rhs) const {
  return type == rhs.type &&
      str == rhs.str;
}
bool Token::operator!=(const Token &rhs) const {
  return !(rhs == *this);
}
bool Token::operator<(const Token &rhs) const {
  if (type < rhs.type)
    return true;
  if (rhs.type < type)
    return false;
  return str < rhs.str;
}
bool Token::operator>(const Token &rhs) const {
  return rhs < *this;
}
bool Token::operator<=(const Token &rhs) const {
  return !(rhs < *this);
}
bool Token::operator>=(const Token &rhs) const {
  return !(*this < rhs);
}
