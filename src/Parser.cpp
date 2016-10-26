//
// Created by 胡一鸣 on 16/10/25.
//

#include "Parser.h"
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "AST.h"
#include "ASTType.h"

using namespace std;

namespace cParser {

ParserFun binaryParser = [](vector<Token *> &tokens, int begin, int end,
                            int position) -> shared_ptr<Statement> {
  auto ast =
      shared_ptr<Statement>(new Statement(ASTType::Binary, tokens[position]));
  ast->children.push_back(Parser::parseTokens(tokens, begin, position));
  ast->children.push_back(Parser::parseTokens(tokens, position + 1, end));
  return ast;
};

ParserFun callParser = [](vector<Token *> &tokens, int begin, int end,
                            int position) -> shared_ptr<Statement> {
  auto ast =
      shared_ptr<Statement>(new Statement(ASTType::Call, tokens[position]));
  int cur_token = begin;
  // TODO: check call error
  // skip (, )
  begin++;
  end--;
  int lastCommaPosition = begin;
  while(cur_token < end) {
    if(tokens[cur_token]->token == TokenType::Comma) {
      Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
      lastCommaPosition = begin;
    }
    cur_token++;
  }
  Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
  return ast;
};

unordered_set<int> Parser::finalTokens = {static_cast<int>(TokenType::Num),
                                          static_cast<int>(TokenType::Comma)};

vector<unordered_set<int>> Parser::priorityTable = {
    {Parser::finalTokens},
    {e(TokenType::Add), e(TokenType::Sub)},
    {e(TokenType::Mul), e(TokenType::Div)},
    {e(TokenType::If), e(TokenType::Switch)}};

unordered_map<int, ParserFun> Parser::unFinalTokenParser = {
    {e(TokenType::Add), binaryParser},
    {e(TokenType::Sub), binaryParser},
    {e(TokenType::Mul), binaryParser},
    {e(TokenType::Div), binaryParser}};

bool Parser::isFinal(TokenType t) {
  return finalTokens.find(static_cast<int>(t)) != finalTokens.end();
}

int Parser::getPriority(TokenType t) {
  for (int i = 0; i < priorityTable.size(); ++i) {
    if (priorityTable[i].find(e(t)) != priorityTable[i].end()) {
      return i;
    }
  }
  return -1;
}

ParserFun Parser::getUnFinalParser(TokenType t) {
  return unFinalTokenParser[static_cast<int>(t)];
}

shared_ptr<Statement> Parser::parseTokens(vector<Token *> &tokens, int begin,
                                          int end) {
  if (end - begin < 1) {
    cout << "no token" << endl;
    return nullptr;
  }
  if (end - begin == 1) {
    return shared_ptr<Statement>(new Statement(ASTType::Final, tokens[begin]));
  }
  if (tokens[begin]->token == TokenType::L_BR && tokens[end-1]->token == TokenType::R_BR) {
    begin++;
    end--;
  }
  int curToken = begin, maxPrioToken = begin;
  int maxPriority = -1;
  int paren = 0;
  while (curToken < tokens.size()) {
    if (tokens[curToken]->token == TokenType::L_BR) {
      paren++;
    }
    else if (tokens[curToken]->token == TokenType::R_BR) {
      paren--;
    }
    else if (paren == 0){
      int priority = getPriority(tokens[curToken]->token);
      if (priority > maxPriority) {
        maxPriority = priority;
        maxPrioToken = curToken;
      }
    }
    curToken++;
  }
  cout << tokens[maxPrioToken]->name << endl;
  auto parser = getUnFinalParser(tokens[maxPrioToken]->token);
  return parser(tokens, begin, end, maxPrioToken);
}
}
