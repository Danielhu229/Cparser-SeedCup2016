//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/25.
//

#ifndef C_PARSER_ASTFACTORY_H
#define C_PARSER_ASTFACTORY_H

#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AST.h"
#include "Token.h"
#include "TokenType.h"

using namespace std;

namespace cParser {

/**
 * the parser function format declartion
 */
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
   */
  static bool isFinal(TokenType t);

  /**
   * get parser function for un-final tokens]
   */
  static ParserFun getUnFinalParser(TokenType t);

  /**
   * get token priority from priorityTable
   */
  static int getPriority(TokenType t);

  /**
   * parse token recurisively
   */
  static Statement *parseTokens(vector<Token *> &tokens, int begin, int end);
};
} // namespace cParser

#endif // C_PARSER_ASTFACTORY_H
