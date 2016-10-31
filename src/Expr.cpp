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
  while (pos < mTokens.size()) {
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
  while (sColonPos < mTokens.size() && mTokens[sColonPos]->type != TokenType::S_Colon) {
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

cParser::Statement *Expr::parseIfExpr() {
  int begin = pos - 1;
  int r_brPos = (int) mTokens.size();
  int sColonPos = pos;
  int end = pos;
  // find the initial pos of if that no else before it.
  int initial = pos;
  while (initial < mTokens.size()) {
    if (mTokens[initial]->type == TokenType::If && mTokens[initial - 1]->type != TokenType::Else) {
      break;
    }
    initial++;
  }
  int elsePos = initial;
  int index = pos;
  // find the last occurrence of else
  while (index < initial) {
    if (mTokens[index]->type == TokenType::Else) {
      elsePos = index;
    }
    index++;
  }

  if (elsePos == initial) {
    // no else branch find,then we just find the last occurrence of '}' or the last occurrence of ';'
    // first make sure if branch has '{'
    int r_phPos = pos;
    // look for ')' first;
    while (r_phPos < initial && mTokens[r_phPos]->type != TokenType::R_PH) {
      r_phPos++;
    }

    int l_brPos = r_phPos + 1;

    if (mTokens[l_brPos]->type != TokenType::L_BR) {
      // no '{' found in if branch, then we find the correct pos of proper ';'
      // TODO: deal with this case:
      // if ()
      //    int a = 1;
      // a++;
      // if () {
      // }

      int keywords = r_phPos + 1;
      if (mTokens[keywords]->type == TokenType::For) {
        // tricky part, we directly find a for,
        // then we should look the last semi-colon that has the same line number with the third semi-colon
        int sColonCount = 0;
        int tmp = keywords;
        while (sColonCount < 3) {
          if (mTokens[tmp]->type == TokenType::S_Colon) {
            sColonPos = tmp;
            sColonCount++;
          }
          tmp++;
        }
        sColonPos = Utility::findLastSColon(mTokens, sColonPos, initial);
      } else {
        sColonPos = Utility::findLastSColon(mTokens, keywords, initial);
      }
    } else {
      // we find a '{' in if branch, we should look for the '}'
      r_brPos = Utility::findBr(mTokens, l_brPos, (int) mTokens.size());
    }
    // find '}'
    if (r_brPos != -1 && r_brPos != mTokens.size()) {
      end = r_brPos + 1;
    } else {
      // '}' not found, use semiColon pos instead
      end = sColonPos + 1;
    }
  } else {

    // we find a else, elsePos now is the last occurrence of keyword else
    int r_phPos;
    int l_brPos;
    // first we check this else
    if (mTokens[elsePos + 1]->type == TokenType::If) {
      // we have a else if in the end
      r_phPos = elsePos + 1;
      while (r_phPos < initial && mTokens[r_phPos]->type != TokenType::R_PH) {
        r_phPos++;
      }
      l_brPos = r_phPos + 1;

      if (mTokens[l_brPos]->type != TokenType::L_BR) {
        // no '{' after else if, find the proper pos of ';'
        int keywords = l_brPos + 1;
        if (mTokens[keywords]->type == TokenType::For) {
          // tricky part, we directly find a for,
          // then we should look the last semi-colon that has the same line number with the third semi-colon
          int sColonCount = 0;
          int tmp = keywords;
          while (sColonCount < 3) {
            if (mTokens[tmp]->type == TokenType::S_Colon) {
              sColonPos = tmp;
              sColonCount++;
            }
            tmp++;
          }
          sColonPos = Utility::findLastSColon(mTokens, sColonPos, initial);
        } else {
          sColonPos = Utility::findLastSColon(mTokens, keywords, initial);
        }
      } else {
        // find '{' after else if
        r_brPos = Utility::findBr(mTokens, elsePos, (int) mTokens.size());
      }
    } else {
      // we only have a else in the end
      l_brPos = elsePos + 1;
      if (mTokens[l_brPos]->type != TokenType::L_BR) {
        // no '{' after else, find the proper pos of ';'
        int keywords = elsePos + 1;
        if (mTokens[keywords]->type == TokenType::For) {
          // tricky part, we directly find a for,
          // then we should look the last semi-colon that has the same line number with the third semi-colon
          int sColonCount = 0;
          int tmp = keywords;
          while (sColonCount < 3) {
            if (mTokens[tmp]->type == TokenType::S_Colon) {
              sColonPos = tmp;
              sColonCount++;
            }
            tmp++;
          }
          sColonPos = Utility::findLastSColon(mTokens, sColonPos, initial);
        } else {
          sColonPos = Utility::findLastSColon(mTokens, keywords, initial);
        }
      } else {
        // find '{' after else
        r_brPos = Utility::findBr(mTokens, elsePos, (int) mTokens.size());
      }
    }
    // find '}'
    if (r_brPos != -1 && r_brPos != mTokens.size()) {
      end = r_brPos + 1;
    } else {
      end = sColonPos + 1;
    }
  }
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

cParser::Statement *Expr::parseWhileExpr() {
  // much like what we do in if expression.
  int begin = pos - 1;
  int sColonCount = 0;
  int r_brPos = (int) mTokens.size();
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
  r_brPos = Utility::findBr(mTokens, index, (int) mTokens.size());
  if (r_brPos == mTokens.size() || r_brPos == -1) {
    end = sColonPos + 1;
  } else {
    end = r_brPos + 1;
  }
  pos = end - 1;
  return cParser::Parser::parseTokens(mTokens, begin, end);
}
