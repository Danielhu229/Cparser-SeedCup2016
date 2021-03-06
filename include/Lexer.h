//
// Created by aLIEzTed on 10/24/16.
//

#ifndef SEEDCUP_PHASER_H
#define SEEDCUP_PHASER_H
#include "Token.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

namespace cParser {

class Lexer {

private:
  const string &code;
  const string old_code;
  int pos;
  bool negative;
  map<string, Token> keywords;
  Token curr_token;
  void init();
  void initFunctions();
  void next();
  void push(Token *token);
  map<int, std::function<void()>> functions;

public:
  map<string, Token> identifiers;
  int line;
  vector<Token *> tokens;
  Lexer(const string &code);
  /*!
   * begin Lexcial analysis;
   */
  void lexan();
  Token INT, DOUBLE, FLOAT, CHAR, LONG, UNSIGNED, BREAK, SWITCH, CASE, IF, ELSE,
      ENUM, RETURN, SIZEOF, WHILE, FOR, VOID, MAIN, ASSIGN, EQ;
  Token INC, DEC;
  Token ADD, SUB, DIV, MUL;
  Token LE, LT, GE, GT, NE, COND;
  Token LOR;
  Token LAN;
  Token NOT;
  Token MOD;
  Token L_BR, R_BR;
  Token L_BRAK, R_BRAK;
  Token L_PH, R_PH;
  Token S_COLON, COLON, COMMA;
  Token DO;
  Token PRINTF;
};
} // namespace cParser

#endif // SEEDCUP_PHASER_H
