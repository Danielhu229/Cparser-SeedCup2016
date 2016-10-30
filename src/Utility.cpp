//
// Created by 胡一鸣 on 16/10/27.
//

#include <Token.h>
#include <TokenType.h>
#include <Utility.h>

using namespace cParser;

vector<Token *> Utility::combineElseIf(vector<Token *> &tokens) {
  vector<Token *> combinedTokens;
  for (int i = 0; i < tokens.size(); ++i) {
    if (tokens[i]->type == TokenType::Else && i + 1 < tokens.size() &&
        tokens[i + 1]->type == TokenType::If) {
      combinedTokens.push_back(new Token("else if", TokenType::ElseIf));
      i++;
    } else {
      combinedTokens.push_back(tokens[i]);
    }
  }
  return combinedTokens;
}

string Utility::intsToString(vector<int>& src) {
  string result;
  for (int num : src) {
    result += to_string(num);
    result += " ";
  }
  if (result.length() > 0) {
    result = result.substr(0, result.length() - 1);
  }
  return result;
}