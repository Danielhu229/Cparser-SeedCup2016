//
// Created by aLIEzTed on 10/24/16.
//

#ifndef SEEDCUP_PHASER_H
#define SEEDCUP_PHASER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "Token.h"
#include <vector>
using namespace std;

namespace cParser {

class Lexer {

 private:
  const string &code;
  const string old_code;
  int pos;

  map<string, Token> keywords;
  Token curr_token;
  void init();
  void initFunctions();
  void next();
  map<int, std::function<void()>> functions;
 public:

  map<string, Token> identifiers;
  int line;
  vector<Token *> tokens;
  Lexer(const string &code);
  /*
   * Lexical analysis
   */
  void lexan();
  void push(Token *token);
  Token INT, DOUBLE, FLOAT, CHAR, LONG, UNSIGNED, BREAK, SWITCH, CASE, IF, ELSE, ENUM, RETURN, SIZEOF, WHILE, FOR, VOID,
      MAIN, ASSIGN, EQ;
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

};

}

#endif //SEEDCUP_PHASER_H
