//
// Created by aLIEzTed on 10/25/16.
//

#include "../include/Token.h"


Token::Token(std::string name, token_type symbol) {
  this->name = name;
  this->token = symbol;
  this->hash = m_hash(this->name);
  this->token_val = "";
  this->clz = 0;
  this->type = 0;
}

Token::Token(std::string name, token_type symbol, std::string value) {
  this->name = name;
  this->token = symbol;
  this->token_val = value;
}

Token::Token(const Token &obj) {
  this->token_val = obj.token_val;
  this->token = obj.token;
  this->name = obj.name;
  this->clz = obj.clz;
  this->type = obj.type;
}

Token::Token() {
  this->token_val = "";
  this->clz = 0;
  this->type = 0;
}
std::ostream &operator<<(std::ostream &os, const Token &token) {
  std::string token_name;
  std::string token_val;
  switch (token.token) {
    case token_type :: Var:
      token_name = "Var";
      token_val = token.name;
      break;
    case token_type:: Int:
      token_name = "Int";
      token_val = token.name;
      break;
    case token_type::Add:
      token_name = "Add";
      token_val = token.name;
      break;
    case token_type ::And:
      token_name = "And";
      token_val = token.name;
      break;
    case token_type::Assign:
      token_name = "Assign";
      token_val = token.name;
      break;
    case token_type ::Break:
      token_name = "Break";
      token_val = token.name;
      break;
    case token_type ::Case:
      token_name = "Case";
      token_val = token.name;
      break;
    case token_type ::Char:
      token_name = "Char";
      token_val = token.name;
      break;
    case token_type::Else:
      token_name = "Else";
      token_val = token.name;
      break;
    case token_type ::Enum:
      token_name = "Enum";
      token_val = token.name;
      break;
    case token_type ::Num:
      token_name = "Num";
      token_val = token.token_val;
      break;
    case token_type ::Str:
      token_name = "Str";
      token_val = token.token_val;
      break;
    case token_type::S_Colon:
      token_name = "Semicolon";
      break;
    case token_type :: Colon:
      token_name = "Colon";
      break;
    case token_type ::Double:
      token_name = "Double";
      break;
    case token_type::Float:
      token_name = "Float";
      break;
    case token_type::L_PH:
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
