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

ParserFun declareVarParser = [](vector<Token *> &tokens, int begin, int end,
                                int position) -> shared_ptr<Statement> {
  shared_ptr<Statement> ast =
      make_shared<Statement>(ASTType::DeclareVar, tokens[position]);
  auto child = Parser::parseTokens(tokens, position + 1, end);
  ast->children.push_back(child);
  return ast;
};

ParserFun colonParser = [](vector<Token *> &tokens, int begin, int end,
                           int position) -> shared_ptr<Statement> {
  shared_ptr<Statement> ast =
      make_shared<Statement>(ASTType::ChildStatement, tokens[position]);
  if (begin < position) {
    auto child = Parser::parseTokens(tokens, begin, position);
    ast->children.push_back(child);
  }
  return ast;
};

ParserFun selfParser = [](vector<Token *> &tokens, int begin, int end,
                          int position) -> shared_ptr<Statement> {
  auto left = Parser::parseTokens(tokens, begin, position);
  auto right = Parser::parseTokens(tokens, position + 1, end);
  if (left && left->token->token == TokenType::Var) {
    shared_ptr<Statement> ast =
        make_shared<Statement>(ASTType::LSelf, tokens[position]);
    ast->children.push_back(left);
    return ast;
  } else if (right && right->token->token == TokenType::Var) {
    shared_ptr<Statement> ast =
        make_shared<Statement>(ASTType::RSelf, tokens[position]);
    ast->children.push_back(right);
    return ast;
  }
};

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
  while (cur_token < end) {
    if (tokens[cur_token]->token == TokenType::Comma) {
      Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
      lastCommaPosition = begin;
    }
    cur_token++;
  }
  Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
  return ast;
};

ParserFun ifParser = [](vector<Token *> &tokens, int begin, int end,
                        int position) -> shared_ptr<Statement> {
  auto ast =
      shared_ptr<Statement>(new Statement(ASTType::If, tokens[position]));
  ast->children.push_back(Parser::parseTokens(tokens, position + 2, end - 1));
  return ast;
};

unordered_set<int> Parser::finalTokens = {
    e(TokenType::Num), e(TokenType::Comma), e(TokenType::Var)};

vector<unordered_set<int>> Parser::priorityTable = {
    {e(TokenType::If), e(TokenType::For), e(TokenType::Switch)},
    {e(TokenType::Colon)},
    {e(TokenType::Assign)},
    {e(TokenType::Float), e(TokenType::Int), e(TokenType::Double),
     e(TokenType::Str)},
    {e(TokenType::Add), e(TokenType::Sub)},
    {e(TokenType::Mul), e(TokenType::Div)},
    {e(TokenType::Inc), e(TokenType::Dec)},
    Parser::finalTokens};

unordered_map<int, ParserFun> Parser::unFinalTokenParser = {
    {e(TokenType::S_Colon), colonParser},
    {e(TokenType::Inc), selfParser},
    {e(TokenType::Dec), selfParser},
    {e(TokenType::Assign), binaryParser},
    {e(TokenType::Add), binaryParser},
    {e(TokenType::Sub), binaryParser},
    {e(TokenType::Mul), binaryParser},
    {e(TokenType::Div), binaryParser},
    {e(TokenType::Eq), binaryParser},
    {e(TokenType::Gt), binaryParser},
    {e(TokenType::Lt), binaryParser},
    {e(TokenType::Ge), binaryParser},
    {e(TokenType::Float), declareVarParser},
    {e(TokenType::Int), declareVarParser},
    {e(TokenType::Double), declareVarParser},
    {e(TokenType::Str), declareVarParser},
    {e(TokenType::Le), binaryParser}};

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
  auto parser = unFinalTokenParser.find(e(t));
  if (parser == unFinalTokenParser.end()) {
    getchar();
    cout << "token has no parser" << endl;
    return nullptr;
  }
  return parser->second;
}

shared_ptr<Statement> Parser::parseTokens(vector<Token *> &tokens, int begin,
                                          int end) {
  if (end - begin < 1) {
    cout << "no token" << endl;
    return nullptr;
  }
  if (end > tokens.size() || begin < 0) {
    cout << "token out of range" << endl;
    return nullptr;
  }
  if (end - begin == 1) {
    return shared_ptr<Statement>(new Statement(ASTType::Final, tokens[begin]));
  }
  if (tokens[begin]->token == TokenType::L_BR &&
      tokens[end - 1]->token == TokenType::R_BR) {
    begin++;
    end--;
  }
  int curToken = begin, minPrioToken = begin;
  int minPriority = 100;
  int paren = 0;
  while (curToken < end) {
    if (tokens[curToken]->token == TokenType::L_BR) {
      paren++;
    } else if (tokens[curToken]->token == TokenType::R_BR) {
      paren--;
    } else if (paren == 0) {
      int priority = getPriority(tokens[curToken]->token);
      if (priority < minPriority) {
        minPriority = priority;
        minPrioToken = curToken;
      }
    }
    curToken++;
  }
  auto parser = getUnFinalParser(tokens[minPrioToken]->token);
  return parser(tokens, begin, end, minPrioToken);
}
}
