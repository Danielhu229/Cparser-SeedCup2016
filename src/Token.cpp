//
// Created by aLIEzTed on 10/25/16.
//

#include "Token.h"


Token::Token(std::string name, TokenType symbol)
    :name(name), token(symbol) {
  this->token_val = "";
  this->clz = 0;
  this->type = 0;
}

Token::Token(std::string name, TokenType symbol, std::string value)
    :name(name), token(symbol), token_val(value) {
}

Token::Token(const Token &obj):token(obj.token) {
  this->token_val = obj.token_val;
  this->name = obj.name;
  this->clz = obj.clz;
  this->type = obj.type;
}

Token::Token():token(TokenType::S_Colon) {
  this->token_val = "";
  this->clz = 0;
  this->type = 0;
}
std::ostream &operator<<(std::ostream &os, const Token &token) {
  std::string token_name;
  std::string token_val;
  switch (token.token) {
    case TokenType :: Var:
      token_name = "Var";
      token_val = token.name;
      break;
    case TokenType:: Int:
      token_name = "Int";
      token_val = token.name;
      break;
    case TokenType::Add:
      token_name = "Add";
      token_val = token.name;
      break;
    case TokenType::Assign:
      token_name = "Assign";
      token_val = token.name;
      break;
    case TokenType ::Break:
      token_name = "Break";
      token_val = token.name;
      break;
    case TokenType ::Case:
      token_name = "Case";
      token_val = token.name;
      break;
    case TokenType ::Char:
      token_name = "Char";
      token_val = token.name;
      break;
    case TokenType::Else:
      token_name = "Else";
      token_val = token.name;
      break;
    case TokenType ::Enum:
      token_name = "Enum";
      token_val = token.name;
      break;
    case TokenType ::Num:
      token_name = "Num";
      token_val = token.token_val;
      break;
    case TokenType ::Str:
      token_name = "Str";
      token_val = token.token_val;
      break;
    case TokenType::S_Colon:
      token_name = "Semicolon";
      break;
    case TokenType :: Colon:
      token_name = "Colon";
      break;
    case TokenType ::Double:
      token_name = "Double";
      break;
    case TokenType::Float:
      token_name = "Float";
      break;
    case TokenType::L_PH:
      token_name = "(";
    default:
      token_name = "Unknown";
      token_val = "N/A";
  }
  os << "<" << token.name << ", " << token.token_val << ">";
  return os;
}

bool Token::operator==(const Token &rhs) const {
  return token == rhs.token &&
      name == rhs.name;
}
bool Token::operator!=(const Token &rhs) const {
  return !(rhs == *this);
}
bool Token::operator<(const Token &rhs) const {
  if (token < rhs.token)
    return true;
  if (rhs.token < token)
    return false;
  return name < rhs.name;
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
