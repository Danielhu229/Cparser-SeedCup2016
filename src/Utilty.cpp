//
// Created by 胡一鸣 on 16/10/27.
//

#include <Utility.h>
#include <TokenType.h>
#include <Token.h>


namespace cParser {
namespace Utility {
  vector<Token*> combineElseIf(vector<Token* >& tokens) {
    vector<Token*> combinedTokens;
  for (int i = 0; i < tokens.size(); ++i) {
    if (tokens[i]->token == TokenType::Else) {
      if (i + 1 < tokens.size() && tokens[i+1]->token == TokenType::If) {
        combinedTokens.push_back(new Token("else if", TokenType::ElseIf));
        i++;
      }
    } else {
      combinedTokens.push_back(tokens[i]);
    }
  }
}
}
}