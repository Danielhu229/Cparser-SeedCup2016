//
// Created by aLIEzTed on 10/29/16.
//

#include "Expr.h"
#include <vector>
#include <Parser.h>
#include <iostream>
#include <stack>
using namespace std;

bool Expr::parse() {
  cParser::Statement* newStat = nullptr;
  while (pos < mTokens.size()) {
    getNextToken();
    newStat = this->parseStatement();
    if (newStat) {
      statements.push_back(newStat);
    }
  }
  return true;
}

cParser::Statement* Expr::parseStatement() {
  switch (curToken->type) {
    case TokenType::Int:return this->parseDeclare();
    case TokenType::If:return this->parseIfExpr();
    case TokenType::While:return this->parseWhileExpr();
    case TokenType::For:return this->parseForExpr();
    case TokenType::Switch:return this->parseSwitchExpr();
    case TokenType::DO:return this->parseDowhileExpr();
    default:return this->parseExpr();
  }
}

cParser::Statement* Expr::parseDeclare() {
  TokenType aheadToken = lookAheadToken(2)->type;
  // func declaration
  if (aheadToken == TokenType::L_PH) {
    return this->parseFuncDeclaration();
  } else {
    return this->parseVarDeclaration();
  }
}

cParser::Statement* Expr::parseVarDeclaration() {
  int begin = pos - 1;
  int sColonPos = pos;
  int end;
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->type != TokenType::S_Colon) {
    sColonPos++;
  }
  end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}


cParser::Statement* Expr::parseDowhileExpr() {
  int begin = pos - 1;
  // simple way to find end, firstly we find the while pos
  int whilePos = pos - 1;

  while (whilePos < mTokens.size() && mTokens[whilePos]->type != TokenType::While) {
    whilePos++;
  }
  if (whilePos == mTokens.size()) {
    return nullptr;
  }
  int sColonPos = whilePos;
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->type != TokenType::S_Colon) {
    sColonPos++;
  }
  if (sColonPos == mTokens.size()) {
    return nullptr;
  }
  int end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}


cParser::Statement* Expr::parseIfExpr() {
  int begin = pos - 1;
  int elsePos = pos;
  int r_brPos = (int) mTokens.size();
  stack<int> brStack;
  int index = pos;
  int sColonPos = pos;
  int end = pos;
  while (elsePos < mTokens.size() && mTokens[elsePos]->type != TokenType::Else) {
    elsePos++;
  }
  // TODO: find last occurrence of ';'
  // no else branch find,then we just find the last occurrence of '}' or the last occurrence of ';'
  if (elsePos == mTokens.size()) {

    while (index < mTokens.size()) {
      if (mTokens[index]->type == TokenType::L_BR) {
        brStack.push(index);
      } else if (mTokens[index]->type == TokenType::R_BR) {
        if (brStack.empty()) {
          // we don't have any L_BR before, something wrong happened!
          break;
        }
        brStack.pop();
        if (brStack.empty()) {
          r_brPos = index;
          break;
        }
      }
      index++;
    }
    // find the last occurrence of '}'
    index = pos;
    while (index < mTokens.size()) {
      //TODO: maybe the last occurrence of ';' would be correct.
      if (mTokens[index]->type == TokenType::S_Colon) {
        // find the first occurrence of ';'
        sColonPos = index;
      }
      index++;
    }
    // find '}'
    if (r_brPos != mTokens.size()) {
      end = r_brPos + 1;
    } else {
      // '}' not found, use semiColon pos instead
      end = sColonPos + 1;
    }
  } else {
    // we find a else.
    while (1) {
      if (elsePos < mTokens.size()
          && (mTokens[elsePos]->type == TokenType::Else && mTokens[elsePos + 1]->type != TokenType::If)) {
        break;
      }
      elsePos++;
    }
    // find a else without an if behind it.
    index = elsePos;
    // clear the stack
    while (!brStack.empty()) {
      brStack.pop();
    }
    // find the last occurrence of '}'
    while (index < mTokens.size()) {
      if (mTokens[index]->type == TokenType::L_BR) {
        brStack.push(index);
      } else if (mTokens[index]->type == TokenType::R_BR) {
        if (brStack.empty()) {
          // we don't have any L_BR before, something wrong happened!
          break;
        }
        brStack.pop();
        if (brStack.empty()) {
          r_brPos = index;
          break;
        }
      }
      index++;
    }
    index = elsePos;
    while (index < mTokens.size()) {
      if (mTokens[index]->type == TokenType::S_Colon) {
        // find the first occurrence of ';'
        sColonPos = index;
      }
      index++;
    }
    // find '}'
    if (r_brPos != mTokens.size()) {
      end = r_brPos + 1;
    } else {
      end = sColonPos + 1;
    }
  }
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement* Expr::parseExpr() {
  int begin = pos - 1;
  int sColonPos = pos;
  int end;
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->type != TokenType::S_Colon) {
    sColonPos++;
  }
  end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

//TODO: implement this one
cParser::Statement* Expr::parseFuncDeclaration() {
  return nullptr;
}

// TODO: implement this one
cParser::Statement* Expr::parseSwitchExpr() {
  return nullptr;
}

cParser::Statement* Expr::parseForExpr() {
  // in for loop if we should look for the last occurrence of ';'
  int begin = pos - 1;
  int end;
  int r_brPos = pos;
  int sColonPos = pos;
  int sColonCount = 0;
  while (r_brPos < mTokens.size() && mTokens[r_brPos]->type != TokenType::R_BR) {
    if (mTokens[r_brPos]->type == TokenType::S_Colon) {
      sColonPos = r_brPos;
      sColonCount++;
    }
    r_brPos++;
  }
  if (r_brPos == mTokens.size()) {
    // no '}' found
    if (sColonCount >= 1) {
      end = sColonPos + 1;
    } else {
      cout << "Incomplete for expression!" << endl;
      end = begin;
    }
  } else {
    // find '}'
    end = r_brPos + 1;
  }
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement* Expr::parseWhileExpr() {
  // much like what we do in if expression.
  int begin = pos - 1;
  int sColonCount = 0;
  int r_brPos = (int) mTokens.size();
  stack<int> brStack;
  int index = pos;
  int sColonPos = pos;
  int end = pos;
  while (index < mTokens.size()) {
    if (mTokens[index]->type == TokenType::S_Colon) {
      sColonPos = index;
      break;
    }
    index++;
  }
  index = pos;
  while (index < mTokens.size()) {
    if (mTokens[index]->type == TokenType::L_BR) {
      brStack.push(index);
    } else if (mTokens[index]->type == TokenType::R_BR) {
      if (brStack.empty()) {
        // we don't have any L_BR before, something wrong happened!
        break;
      }
      brStack.pop();
      if (brStack.empty()) {
        r_brPos = index;
        break;
      }
    }
    index++;
  }
  if (r_brPos == mTokens.size()) {
    end = sColonPos + 1;
  } else {
    end = r_brPos + 1;
  }
  pos = end - 1;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}
