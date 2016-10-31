//
// Created by 胡一鸣 on 16/10/27.
//

#include <Token.h>
#include <TokenType.h>
#include <Utility.h>
#include <stack>
#include <iostream>
using namespace cParser;

vector<Token *> Utility::combineElseIf(vector<Token *> &tokens) {
  vector<Token *> combinedTokens;
  for (int i = 0; i < static_cast<int>(tokens.size()); ++i) {
    if (tokens[i]->type == TokenType::Else && i + 1 < static_cast<int>(tokens.size()) &&
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

int Utility::findBr(vector<Token *> &tokens, int begin, int end) {
  stack<int> brStack;
  int brPos = -1;
  int la = begin;
  while (la < end) {
    if (tokens[la]->type == TokenType::L_BR) {
      brStack.push(la);
    } else if (tokens[la]->type == TokenType::R_BR) {
      if (brStack.empty()) {
        cout << "Invalid input" << endl;
        break;
      } else {
        brStack.pop();
      }
      if (brStack.empty()) {
        brPos = la;
        break;
      }
    }
    la++;
  }
  return brPos;
}

int Utility::findLastSColon(vector<Token *> &tokens, int begin, int end) {
  int pos = begin;
  int sColonPos = begin;
  while (pos < end) {
    if (tokens[pos]->type == TokenType::S_Colon) {
      // find the first occurrence of ';'
      int tmp = pos;
      while (tmp < end && tokens[tmp]->type == TokenType::S_Colon && tokens[tmp]->lineNum == tokens[pos]->lineNum) {
        tmp++;
      }
      sColonPos = tmp - 1;
      break;
    }
    pos++;
  }
  return sColonPos;
}