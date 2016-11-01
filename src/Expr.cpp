//
// Created by aLIEzTed on 10/29/16.
//

#include "Expr.h"
#include <vector>
#include <Parser.h>
#include <iostream>
#include <Utility.h>
using namespace std;
using namespace cParser;

bool Expr::parse() {
  cParser::Statement *newStat = nullptr;
  while (pos < static_cast<int>(mTokens.size())) {
    getNextToken();
    newStat = this->parseStatement();
    if (newStat) {
      statements.push_back(newStat);
    }
  }
  return true;
}

cParser::Statement *Expr::parseStatement() {
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

cParser::Statement *Expr::parseDeclare() {
  TokenType aheadToken = lookAheadToken(2)->type;
  // func declaration
  if (aheadToken == TokenType::L_PH) {
    return this->parseFuncDeclaration();
  } else {
    return this->parseVarDeclaration();
  }
}

cParser::Statement *Expr::parseVarDeclaration() {
  int begin = pos - 1;
  int sColonPos = pos;
  int end;
  while (sColonPos < static_cast<int>(mTokens.size()) && mTokens[sColonPos]->type != TokenType::S_Colon) {
    sColonPos++;
  }
  end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement *Expr::parseDowhileExpr() {
  int begin = pos - 1;
  // simple way to find end, firstly we find the while pos
  int whilePos = pos - 1;
  int l_brPos = pos;
  int brPos;
  int sColonPos;
  int end;
  if (mTokens[l_brPos]->type == TokenType::L_BR) {
    // find a '{' at the outer part of do
    brPos = Utility::findBr(mTokens, l_brPos, (int) mTokens.size());
    whilePos = brPos;
    while (whilePos < static_cast<int>(mTokens.size())) {
      if (mTokens[whilePos]->type == TokenType::While)
        break;
      whilePos++;
    }
    sColonPos = Utility::findLastSColon(mTokens, whilePos, (int) mTokens.size());
    end = sColonPos + 1;
  } else {
    // no '{' found
    int doPos = pos;
    while (doPos < static_cast<int>(mTokens.size())) {
      if (mTokens[doPos]->type == TokenType::For && mTokens[doPos - 1]->type == TokenType::S_Colon) {
        break;
      }
      doPos++;
    }
    end = doPos;
  }
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement *Expr::parseIfExpr() {
  int begin = pos - 1;
  int end = pos;
  end = Utility::findLastElse(mTokens, pos, (int) mTokens.size());
  // find the initial pos of if that no else before it.
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement *Expr::parseExpr() {
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
cParser::Statement *Expr::parseFuncDeclaration() {
  return nullptr;
}

// TODO: implement this one
cParser::Statement *Expr::parseSwitchExpr() {
  return nullptr;
}

cParser::Statement *Expr::parseForExpr() {
  // in for loop if we should look for the last occurrence of ';'
  int begin = pos - 1;
  int end;
  int r_brPos = -1;
  int sColonPos = pos;
  int r_phPos = begin;
  while (r_phPos < (int) mTokens.size()) {
    if (mTokens[r_phPos]->type == TokenType::R_PH) {
      break;
    }
    r_phPos++;
  }
  int l_brPos = r_phPos + 1;
  if (mTokens[l_brPos]->type == TokenType::L_BR) {
    r_brPos = Utility::findBr(mTokens, r_phPos, (int) mTokens.size());
  } else {
    // we find no '{' in the outer for loop
    // then find have to check the next token
    int forPos = r_phPos;
    while (forPos < mTokens.size()) {
      if (mTokens[forPos]->type == TokenType::For && mTokens[forPos - 1]->type == TokenType::S_Colon) {
        break;
      }
      forPos++;
    }
    int tmp = forPos - 1;
    while (mTokens[tmp]->type == TokenType::S_Colon
        && mTokens[tmp - 1]->type == TokenType::S_Colon
        && mTokens[tmp]->lineNum == mTokens[tmp - 1]->lineNum) {
      tmp--;
    }
    sColonPos = tmp;
  }
  if (r_brPos == -1) {
    // no '}' found
    end = sColonPos + 1;
  } else {
    // find '}'
    end = r_brPos + 1;
  }
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

cParser::Statement *Expr::parseWhileExpr() {
  // much like what we do in if expression.
  int begin = pos - 1;
  int doPos = pos - 1;
  int r_brPos = -1;
  int sColonPos = pos;
  int end = pos;
  int r_phPos = -1;
  int tmp = pos;
  stack<int> phStack;
  while (tmp < mTokens.size()) {
    if (mTokens[tmp]->type ==TokenType::L_PH) {
      phStack.push(tmp);
    }
    if (mTokens[tmp]->type == TokenType::R_PH) {
      phStack.pop();
      if (phStack.empty()) {
        r_phPos = tmp;
        break;
      }
    }
    tmp++;
  }

  while (doPos < mTokens.size() && mTokens[doPos]->type != TokenType::DO) {
    doPos++;
  }

  if (mTokens[r_phPos + 1]->type == TokenType::L_BR) {
    r_brPos = Utility::findBr(mTokens, r_phPos + 1, (int) mTokens.size());
  } else {
    // no brackets in while
    int keywords = r_phPos + 1;
    if (mTokens[keywords]->type == TokenType::DO) {
      // we find do in the while body
      r_phPos = keywords;
      while (r_phPos < mTokens.size() && mTokens[r_phPos]->type != TokenType::R_PH) {
        r_phPos++;
      }
      sColonPos = Utility::findLastSColon(mTokens, r_phPos + 1, (int) mTokens.size());
    } else {
      // no do in the while body
      sColonPos = Utility::findLastSColon(mTokens, r_phPos + 1, (int) mTokens.size());
    }
  }
  if (r_brPos == -1) {
    end = sColonPos + 1;
  } else {
    end = r_brPos + 1;
  }
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}
