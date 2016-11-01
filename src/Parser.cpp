//
// Created by 胡一鸣 on 16/10/25.
//

#include "Parser.h"
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <Utility.h>

#include "AST.h"
#include "ASTType.h"

using namespace std;

namespace cParser {

ParserFun declareVarParser = [](vector<Token *> &tokens, int begin, int end,
                                int position) -> Statement * {
  Statement *ast =
      new Statement(ASTType::DeclareVar, *tokens[position]);
  int lastBegin = position + 1;
  for (int i = lastBegin; i < end; ++i) {
    if (tokens[i]->type == TokenType::Comma) {
      auto child = Parser::parseTokens(tokens, lastBegin, i);
      ast->children.push_back(child);
      lastBegin = i + 1;
    }
  }
  auto child = Parser::parseTokens(tokens, lastBegin, end);
  ast->children.push_back(child);
  return ast;
};

ParserFun colonParser = [](vector<Token *> &tokens, int begin, int end,
                           int position) -> Statement * {
  Statement *ast =
      new Statement(ASTType::ChildStatement, *tokens[position]);
  if (begin < position) {
    auto child = Parser::parseTokens(tokens, begin, position);
    ast->children.push_back(child);
  } else {
    cout << "empty colon" << endl;
  }
  return ast;
};

ParserFun commaParser = [](vector<Token *> &tokens, int begin, int end,
                           int position) -> Statement * {
  Statement *ast =
      new Statement(ASTType::Comma, *tokens[position]);
  int lastCommaPosition = begin - 1;
  int cur_token = begin;
  int paren = 0;
  while (cur_token < end) {
    if (tokens[cur_token]->type == TokenType::L_PH) {
      paren++;
    } else if (tokens[cur_token]->type == TokenType::R_PH) {
      paren--;
    } else if (paren == 0 && tokens[cur_token]->type == TokenType::Comma) {
       ast->children.push_back(Parser::parseTokens(tokens, lastCommaPosition + 1, cur_token));
      lastCommaPosition = cur_token;
    }
    cur_token++;
  }
  ast->children.push_back(Parser::parseTokens(tokens, lastCommaPosition + 1, cur_token));
  return ast;
};

ParserFun selfParser = [](vector<Token *> &tokens, int begin, int end,
                          int position) -> Statement * {
  auto left = Parser::parseTokens(tokens, begin, position);
  auto right = Parser::parseTokens(tokens, position + 1, end);
  if (left && left->token.type == TokenType::Var) {
    Statement *ast =
        new Statement(ASTType::RSelf, *tokens[position]);
    ast->children.push_back(left);
    return ast;
  } else if (right && right->token.type == TokenType::Var) {
    Statement *ast =
        new Statement(ASTType::LSelf, *tokens[position]);
    ast->children.push_back(right);
    return ast;
  }
  cerr << "self operation parser error, return nullptr" << endl;
  return nullptr;
};

ParserFun binaryParser = [](vector<Token *> &tokens, int begin, int end,
                            int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::Binary, *tokens[position]));
  ast->children.push_back(Parser::parseTokens(tokens, begin, position));
  ast->children.push_back(Parser::parseTokens(tokens, position + 1, end));
  return ast;
};

ParserFun exprParser = [](vector<Token *> &tokens, int begin, int end,
                          int position) -> Statement * {
  int fixedPosition = position;
  int priority = Parser::getPriority(tokens[position]->type);
  int paren = 0;
  for (int i = end - 1; i >= begin; i--) {
    if (tokens[i]->type == TokenType::L_PH) {
      paren++;
    } else if (tokens[i]->type == TokenType::R_PH) {
      paren--;
    } else if (paren == 0 && priority == Parser::getPriority(tokens[i]->type)) {
      fixedPosition = i;
      break;
    }
  }
  auto ast = (new Statement(ASTType::Binary, *tokens[fixedPosition]));
  ast->children.push_back(Parser::parseTokens(tokens, begin, fixedPosition));
  ast->children.push_back(Parser::parseTokens(tokens, fixedPosition + 1, end));
  return ast;
};

/*
 * Function for parsing block
 * "{" { <Expression> } "}"
 */
Statement *Parser::blockParser(vector<Token *> &tokens, int begin, int end,
                               int position) {
  if (begin > end)
    return nullptr;
  auto ast =
      new Statement(ASTType::Block, *tokens[position]);

  if (tokens[begin + 1]->type == TokenType::L_BR && tokens[end - 2]->type == TokenType::R_BR) {
    ast->children.push_back(Parser::blockParser(tokens, begin + 1, end - 1, begin + 1));
    return ast;
  }

  int index = begin + 1; // skip '{'
  // the tokens[end - 1] will be '}', we just iterate the inner part of '{ }'
  while (index < end - 1) {
    if (tokens[index]->type == TokenType::If) {
      int elsePos = Utility::findLastElse(tokens, index + 1, end - 1);
      ast->children.push_back(Parser::parseTokens(tokens, index, elsePos + 1));
      index = elsePos + 1;
    } else if (tokens[index]->type == TokenType::For) {
      int brPos = Utility::findBr(tokens, index, end - 1);
      if (brPos != -1 && brPos != end) {
        ast->children.push_back(Parser::parseTokens(tokens, index, brPos + 1));
        index = brPos + 1;
      } else {
        int sColonCount = 0;
        int sColonPos = -1;
        int pos = index + 1;
        while (pos < end) {
          if (tokens[pos]->type == TokenType::S_Colon) {
            sColonPos = pos;
            sColonCount++;
          }
          if (sColonCount == 3) {
            break;
          }
          pos++;
        }
        // find the third of ';'
        if (sColonCount == 3 && sColonPos != -1) {
          ast->children.push_back(Parser::parseTokens(tokens, index, sColonPos + 1));
          index = sColonPos + 1;
        }
      }
    } else if (tokens[index]->type == TokenType::While) {
      int brPos = Utility::findBr(tokens, index, end - 1);
      if (brPos != -1 && brPos != end) {
        ast->children.push_back(Parser::parseTokens(tokens, index, brPos + 1));
        index = brPos + 1;
      } else {
        int sColonPos = -1;
        int pos = index + 1;
        while (pos < end) {
          if (tokens[pos]->type == TokenType::S_Colon) {
            sColonPos = pos;
            break;
          }
          pos++;
        }
        if (sColonPos != -1 && sColonPos != end) {
          ast->children.push_back(Parser::parseTokens(tokens, index, sColonPos + 1));
          index = sColonPos + 1;
        } else {
          cout << "Invalid input!" << endl;
          ast->children.push_back(nullptr);
        }
      }
    } else if (tokens[index]->type == TokenType::L_BR) {
      int brPos = Utility::findBr(tokens, index, end - 1);
      if (brPos != -1) {
        // we find another brackets
        ast->children.push_back(Parser::blockParser(tokens, index + 1, brPos + 1, index));
        index = brPos + 1;
      }
    } else if (tokens[index]->type == TokenType::DO) {
      // find the while
      int sColonPos = index;
      int whilePos = index;
      int tmp = whilePos;
      while (tmp < end) {
        if (tokens[tmp]->type == TokenType::While)
          whilePos = tmp;
        tmp++;
      }
      sColonPos = Utility::findLastSColon(tokens, whilePos, end);
      ast->children.push_back(Parser::parseTokens(tokens, index, sColonPos + 1));
      index = sColonPos + 1;
    } else if (tokens[index]->type == TokenType::Else) {
      index++;
    } else {
      // try find the occurrence of ';'
      int sColonPos = index;
      while (sColonPos < end) {
        if (tokens[sColonPos]->type == TokenType::S_Colon) {
          break;
        }
        sColonPos++;
      }
      sColonPos = Utility::findLastSColon(tokens, index, end);
      ast->children.push_back(Parser::parseTokens(tokens, index, sColonPos + 1));
      index = sColonPos + 1;
    }
  }

  return ast;
};

/*
 * Function for parsing "for" expression
 * "for" "(" <Expression> ";" <Expression> ";" <Expression> ")" { <Block> |
 * <Expression> }
 */
ParserFun forParser = [](vector<Token *> &tokens, int begin, int end,
                         int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::For, *tokens[position]));
  int prev = position + 2;
  int scolonPos = position + 1;
  // fixme: Error handling if have ';' more than twice
  while (tokens[scolonPos]->type != TokenType::R_PH && scolonPos < end) {
    if (tokens[scolonPos]->type == TokenType::S_Colon) {
      ast->children.push_back(Parser::parseTokens(tokens, prev, scolonPos + 1));
      prev = scolonPos + 1;
    }
    scolonPos++;
  }

  /*
   * tricky part of for loop, we add a fake ';' at the end of condition.
   * remove the fake ';' after parsing it.
   */
  tokens.insert(tokens.begin() + scolonPos, new Token(";", TokenType::S_Colon));
  ast->children.push_back(Parser::parseTokens(tokens, prev, scolonPos + 1));
  tokens.erase(tokens.begin() + scolonPos + 1);
  // minus 1 because offset by one!
  if (tokens[scolonPos + 1]->type == TokenType::L_BR) {
    ast->children.push_back(Parser::blockParser(tokens, scolonPos + 1, end, scolonPos + 1));
  } else {
    ast->children.push_back(Parser::parseTokens(tokens, scolonPos + 1, end));
  }
  return ast;
};

// TODO: implement this method.
ParserFun switchParser = [](vector<Token *> &tokens, int begin, int end,
                            int position) -> Statement * {
  auto ast = new Statement(ASTType::Switch, *tokens[position]);
  return ast;

};

/*
 * Function for parsing "do...while" expression
 */
ParserFun dowhileParser = [](vector<Token *> &tokens, int begin, int end,
                             int position) -> Statement * {
  auto ast = new Statement(ASTType::Do, *tokens[position]);
  int whilePos = position + 1; // plus 1 to skip 'do'
  int tmp = whilePos;
  while (tmp < end) {
    if (tokens[tmp]->type == TokenType::While) {
      whilePos = tmp;
    }
    tmp++;
  }

  // skip since we don't need to parse while
  ast->children.push_back(Parser::parseTokens(tokens, whilePos + 1, end));

  int doPos = position + 1;
  if (tokens[doPos]->type == TokenType::L_BR) {
    int brPos = Utility::findBr(tokens, doPos, whilePos);
    if (brPos != -1) {
      ast->children.push_back(Parser::blockParser(tokens, doPos, brPos, doPos));
    } else {
      cout << "invalid input";
      ast->children.push_back(nullptr);
    }
  } else {
    ast->children.push_back(Parser::parseTokens(tokens, position + 1, whilePos));
  }
  return ast;
};

/*
 * Function for parsing "for" expression
 * "while" "(" <Expression> ")" { <Block> | <Expression> }
 */
ParserFun whileParser = [](vector<Token *> &tokens, int begin, int end,
                           int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::While, *tokens[position]));
  int rPos = position + 2; // plus 1 to skip 'while' and ('
  int balance = 1;
  while (balance > 0) {
    if (tokens[rPos]->type == TokenType::L_PH) {
      balance++;
    } else if (tokens[rPos]->type == TokenType::R_PH) {
      balance--;
    }
    rPos++;
  }
  rPos--; // back behind )
  ast->children.push_back(Parser::parseTokens(tokens, position + 2, rPos));

  if (tokens[rPos + 1]->type == TokenType::L_BR) {
    ast->children.push_back(Parser::blockParser(tokens, rPos + 1, end, rPos + 1));
  } else {
    ast->children.push_back(Parser::parseTokens(tokens, rPos + 1, end));
  }
  return ast;
};

ParserFun callParser = [](vector<Token *> &tokens, int begin, int end,
                          int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::Call, *tokens[position]));
  int cur_token = begin;
  // TODO: check call error
  // skip (, )
  begin++;
  end--;
  int lastCommaPosition = begin;
  while (cur_token < end) {
    if (tokens[cur_token]->type == TokenType::Comma) {
      Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
      lastCommaPosition = begin;
    }
    cur_token++;
  }
  Parser::parseTokens(tokens, lastCommaPosition, cur_token - 1);
  return ast;
};

ParserFun printfParser = [](vector<Token *> &tokens, int begin, int end,
                          int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::Printf, *tokens[position]));
  int cur_token = begin;
  // skip (, )
  begin += 2;
  end--;
  ast->children.push_back(Parser::parseTokens(tokens, begin, end));
  return ast;
};

/*
 * Function for parsing "if" expression
 * "if" "(" <Expression> ")" { <Block> | <Expression> }
 */
ParserFun ifParser = [](vector<Token *> &tokens, int begin, int end,
                        int position) -> Statement * {
  auto ast =
      (new Statement(ASTType::If, *tokens[position]));
  int elsePos = position;

  int rPos = position + 1; // plus 1 to skip 'if'
  while (rPos < end && tokens[rPos]->type != TokenType::R_PH) {
    rPos++;
  }

  ast->children.push_back(Parser::parseTokens(tokens, position + 2, rPos));
  stack<int> ifStack;
  int pos = position;
  while (pos < end) {
    if (tokens[pos]->type == TokenType::If) {
      ifStack.push(pos);
    } else if (tokens[pos]->type == TokenType::Else) {
      ifStack.pop();
      if (ifStack.empty()) {
        elsePos = pos;
        break;
      }
    }
    pos++;
  }
  if (!ifStack.empty()) {
    if (tokens[end - 1]->type == TokenType::R_BR) {
      ast->children.push_back(Parser::blockParser(tokens, rPos + 1, end, rPos + 1));
    } else {
      ast->children.push_back(Parser::parseTokens(tokens, rPos + 1, end));
    }
    ast->children.push_back(nullptr);
  } else {
    if (tokens[elsePos - 1]->type == TokenType::R_BR) {
      ast->children.push_back(Parser::blockParser(tokens, rPos + 1, elsePos, rPos + 1));
    } else {
      ast->children.push_back(Parser::parseTokens(tokens, rPos + 1, elsePos));
    }
    if (tokens[elsePos + 1]->type == TokenType::L_BR) {
      ast->children.push_back(Parser::blockParser(tokens, elsePos + 1, end, elsePos + 1));
    } else {
      ast->children.push_back(Parser::parseTokens(tokens, elsePos + 1, end));
    }
  }
  return ast;
};

unordered_set<int> Parser::finalTokens = {
    e(TokenType::Num),
    e(TokenType::Break),
    e(TokenType::Var),
    e(TokenType::Str)
};

// we ignore the '}' so we don't need to put '}' in this table
vector<unordered_set<int>> Parser::priorityTable = {
    {e(TokenType::If), e(TokenType::For), e(TokenType::Switch),
     e(TokenType::Else),
     e(TokenType::DO),
     e(TokenType::While)},
    {e(TokenType::Colon)},
    {e(TokenType::S_Colon)},
    {e(TokenType::Float), e(TokenType::Int), e(TokenType::Double)},
    {e(TokenType::Comma)},
    {e(TokenType::Assign)},
    {e(TokenType::Lt), e(TokenType::Gt), e(TokenType::Le), e(TokenType::Ge), e(TokenType::Ne), e(TokenType::Eq)},
    {e(TokenType::Add), e(TokenType::Sub)},
    {e(TokenType::Mul), e(TokenType::Div)},
    {e(TokenType::Inc), e(TokenType::Dec)},
    {e(TokenType::R_BR)}, {e(TokenType::L_BR)},
    {e(TokenType::Printf)},
    Parser::finalTokens
};

unordered_map<int, ParserFun> Parser::unFinalTokenParser = {
    {e(TokenType::S_Colon), colonParser},
    {e(TokenType::Inc), selfParser},
    {e(TokenType::Dec), selfParser},
    {e(TokenType::Assign), binaryParser},
    {e(TokenType::Add), exprParser},
    {e(TokenType::Sub), exprParser},
    {e(TokenType::Mul), exprParser},
    {e(TokenType::Div), exprParser},
    {e(TokenType::Eq), binaryParser},
    {e(TokenType::Ne), binaryParser},
    {e(TokenType::Gt), binaryParser},
    {e(TokenType::Lt), binaryParser},
    {e(TokenType::Ge), binaryParser},
    {e(TokenType::Le), binaryParser},
    {e(TokenType::Float), declareVarParser},
    {e(TokenType::Int), declareVarParser},
    {e(TokenType::Double), declareVarParser},
    {e(TokenType::Printf), printfParser},
    {e(TokenType::Comma), commaParser},
    {e(TokenType::L_BR), blockParser},
    {e(TokenType::If), ifParser},
    {e(TokenType::DO), dowhileParser},
    {e(TokenType::While), whileParser},
    {e(TokenType::For), forParser}};

bool Parser::isFinal(TokenType t) {
  return finalTokens.find(static_cast<int>(t)) != finalTokens.end();
}

int Parser::getPriority(TokenType t) {
  for (int i = 0; i < static_cast<int>(priorityTable.size()); ++i) {
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

Statement *Parser::parseTokens(vector<Token *> &tokens, int begin,
                               int end) {
  if (end - begin < 1) {
    return nullptr;
  }
  if (end > static_cast<int>(tokens.size()) || begin < 0) {
    cout << "token out of range" << endl;
    return nullptr;
  }
  if (end - begin == 1) {
    return new Statement(ASTType::Final, *tokens[begin]);
  }
  // ignore '(' and ')' outside
  if (tokens[begin]->type == TokenType::L_PH &&
      tokens[end - 1]->type == TokenType::R_PH) {
    begin++;
    end--;
  }
  int curToken = begin, minPrioToken = begin;
  int minPriority = 100;
  int paren = 0;
  while (curToken < end) {
    // ignore '(' and ')' inside
    if (tokens[curToken]->type == TokenType::L_PH) {
      paren++;
    } else if (tokens[curToken]->type == TokenType::R_PH) {
      paren--;
    } else if (paren == 0) {
      int priority = getPriority(tokens[curToken]->type);
      if (priority < minPriority) {
        minPriority = priority;
        minPrioToken = curToken;
      }
    }
    curToken++;
  }
  auto parser = getUnFinalParser(tokens[minPrioToken]->type);
  return parser(tokens, begin, end, minPrioToken);
}
}
