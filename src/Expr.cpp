//
// Created by aLIEzTed on 10/29/16.
//

#include "Expr.h"
#include <vector>
#include <Parser.h>
#include <iostream>
using namespace std;

bool Expr::parse() {
  shared_ptr<cParser::Statement> newStat = nullptr;
  while (pos < mTokens.size()) {
    getNextToken();
    newStat = this->parseStatement();
    if (newStat) {
      statements.push_back(newStat);
    }
  }
  return true;
}

shared_ptr<cParser::Statement> Expr::parseStatement() {
  switch (curToken->token) {
    case TokenType::Int:return this->parseDeclare();
    case TokenType::If:return this->parseIfExpr();
    case TokenType::While:return this->parseWhileExpr();
    case TokenType::For:return this->parseForExpr();
    case TokenType::Switch:return this->parseSwitchExpr();
    default:return this->parseExpr();
  }
}

shared_ptr<cParser::Statement> Expr::parseDeclare() {
  TokenType aheadToken = lookAheadToken(2)->token;
  // func declaration
  if (aheadToken == TokenType::L_PH) {
    return this->parseFuncDeclaration();
  } else {
    return this->parseVarDeclaration();
  }
}

shared_ptr<Statement> Expr::parseVarDeclaration() {
  int begin = pos - 1;
  int sColonPos = pos;
  int end;
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->token != TokenType::S_Colon) {
    sColonPos++;
  }
  end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

shared_ptr<cParser::Statement> Expr::parseIfExpr() {
  int begin = pos - 1;
  int elsePos = pos;
  int r_brPos = pos;
  int sColonCount = 0;
  int sColonPos = (int) (mTokens.size() - 1);
  int end = pos;
  while (elsePos < mTokens.size() && mTokens[elsePos]->token != TokenType::Else) {
    elsePos++;
  }
  // no else branch find,then we just find the first occurrence of '}' or ';'
  if (elsePos == mTokens.size()) {
    while (r_brPos < mTokens.size() && mTokens[r_brPos]->token != TokenType::R_BR) {
      if (mTokens[r_brPos]->token == TokenType::S_Colon) {
        // find the first occurrence of ';'
        if (sColonPos > r_brPos) {
          sColonPos = r_brPos;
        }
        sColonCount++;
      }
      r_brPos++;
    }
    // find '}'
    if (r_brPos != mTokens.size()) {
      end = r_brPos + 1;
      // in case you have an empty statements like ';;'
    } else if (sColonCount >= 1) {
      end = sColonPos + 1;
    } else {
      cout << "Incomplete input in if expr" << endl;
      end = begin;
    }
  } else {
    // we find a else.
    while (1) {
      if (elsePos < mTokens.size()
          && (mTokens[elsePos]->token == TokenType::Else && mTokens[elsePos + 1]->token != TokenType::If)) {
        break;
      }
      elsePos++;
    }
    // find a else without an if behind it.
    sColonPos = r_brPos = elsePos;
    while (r_brPos < mTokens.size() && mTokens[r_brPos]->token != TokenType::R_BR) {
      if (mTokens[r_brPos]->token == TokenType::S_Colon) {
        sColonPos = r_brPos;
        sColonCount++;
      }
      r_brPos++;
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

shared_ptr<cParser::Statement> Expr::parseExpr() {
  int begin = pos - 1;
  int sColonPos = pos;
  int end;
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->token != TokenType::S_Colon) {
    sColonPos++;
  }
  end = sColonPos + 1;
  pos = end;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}

//TODO: implement this one
shared_ptr<Statement> Expr::parseFuncDeclaration() {
  return shared_ptr<cParser::Statement>();
}

// TODO: implement this one
shared_ptr<cParser::Statement> Expr::parseSwitchExpr() {
  return shared_ptr<Statement>();
}

shared_ptr<cParser::Statement> Expr::parseForExpr() {
  // in for loop if we should look for the last occurrence of ';'
  int begin = pos - 1;
  int end;
  int r_brPos = pos;
  int sColonPos = pos;
  int sColonCount = 0;
  while (r_brPos < mTokens.size() && mTokens[r_brPos]->token != TokenType::R_BR) {
    if (mTokens[r_brPos]->token == TokenType::S_Colon) {
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

shared_ptr<cParser::Statement> Expr::parseWhileExpr() {
  // much like what we do in if expression.
  int begin = pos - 1;
  int end;
  int r_brPos = pos;
  int sColonPos = (int) (mTokens.size() - 1);
  int sColonCount = 0;
  while (r_brPos < mTokens.size() && mTokens[r_brPos]->token != TokenType::R_BR) {
    if (mTokens[r_brPos]->token == TokenType::S_Colon) {
      if (sColonPos > r_brPos) {
        sColonPos = r_brPos;
      }
      sColonCount++;
    }
    r_brPos++;
  }
  if (r_brPos == mTokens.size()) {
    if (sColonCount >= 1) {
      end = sColonPos + 1;
    } else {
      cout << "Incomplete while expression!" << endl;
      end = begin;
    }
  } else {
    end = r_brPos + 1;
  }
  pos = end - 1;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}
