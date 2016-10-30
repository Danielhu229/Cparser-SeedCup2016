//
// Created by 胡一鸣 on 16/10/25.
//

#ifndef C_PARSER_ASTFACTORY_H
#define C_PARSER_ASTFACTORY_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <iostream>

#include "AST.h"
#include "Token.h"
#include "TokenType.h"

using namespace std;

namespace cParser {

typedef function<Statement *(vector<Token *> &tokens, int begin, int end,
                             int position)>
    ParserFun;

constexpr int e(TokenType t) { return static_cast<int>(t); }

class Parser {
  static unordered_set<int> finalTokens;
  static vector<unordered_set<int>> priorityTable;

 public:
  static unordered_map<int, ParserFun> unFinalTokenParser;
  /**
   * check if specific token cannot generate lower expression
   * @param  t [TokenType] specific token
   * @return [bool]
   */
  static bool isFinal(TokenType t);

  static ParserFun getUnFinalParser(TokenType t);

  static int getPriority(TokenType t);

  /**
   * parse token recurisive
   */
  static Statement *parseTokens(vector<Token *> &tokens, int begin,
                                int end);

  static Statement *blockParser(vector<Token *> &tokens, int begin, int end,
                                int position);

};
}

#endif // C_PARSER_ASTFACTORY_H
