//
// Created by 胡一鸣 on 16/10/27.
//

#ifndef C_PARSER_UTILITY_H
#define C_PARSER_UTILITY_H

#include <vector>
#include "Token.h"

using namespace std;

namespace cParser {
namespace Utility {
  /*
   *
   */
  vector<Token*> combineElseIf(vector<Token*>& tokens);
  /*!
   * print all int in the vector to a string in sequentially.
   * @param src the int vector
   * @return the int string
   */
  string intsToString(vector<int>& src);
  /*!
   * find the very last position of a }
   * @param tokens the token stream
   * @param begin the begin of stream used here, included
   * @param end the end of stream used here, excluded
   * @return the very last position of }
   */
  int findBr(vector<Token *> &tokens, int begin, int end);
  /*!
   * find the very last position of ; in one line,
   * this is used for avoid split multiple semi-colon which are in the same line.
   * @param tokens the token stream
   * @param begin the begin of stream used here, included
   * @param end the end of stream used here, excluded
   * @return the very last position of ; which has same line number as the first ; we come accross
   */
  int findLastSColon(vector<Token *> &tokens, int begin, int end);
  /*!
   * find the first position of )
   * @param tokens the token stream
   * @param begin the begin of stream used here, included
   * @param end the end of stream used here, excluded
   * @return the very first position of )
   */
  int findRph(vector<Token *> &tokens, int begin, int end);
  /*!
   * find the correct position of when an if statement ends
   * note that it is complicated to find a position that an if ends
   * @param tokens the token stream
   * @param begin the begin of stream used here, included, you should not include the first if in the begin.
   * @param end the end of stream used here, excluded
   * @return the position of when an if statement ends.
   */
  int findLastElse(vector<Token *> &tokens, int begin, int end);
}
}

#endif //C_PARSER_UTILITY_H
