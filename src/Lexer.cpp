//
// Created by aLIEzTed on 10/24/16.
//

#include "Lexer.h"
#include <functional>
using namespace std;
using namespace cParser;

void Lexer::init() {
  INT = Token("int", TokenType::Int);
  DOUBLE = Token("double", TokenType::Double);
  FLOAT = Token("float", TokenType::Float);
  LONG = Token("long", TokenType::Long);
  CHAR = Token("char", TokenType::Char);
  BREAK = Token("break", TokenType::Break);
  SWITCH = Token("switch", TokenType::Switch);
  CASE = Token("case", TokenType::Case);
  ELSE = Token("else", TokenType::Else);
  ENUM = Token("enum", TokenType::Enum);
  IF = Token("if", TokenType::If);
  RETURN = Token("return", TokenType::Return);
  SIZEOF = Token("sizeof", TokenType::Sizeof);
  WHILE = Token("while", TokenType::While);
  FOR = Token("for", TokenType::For);
  VOID = Token("void", TokenType::Void);
  MAIN = Token("main", TokenType::Main);
  UNSIGNED = Token("unsigned", TokenType::Unsigned);
  DO = Token("do", TokenType::DO);
  ASSIGN = Token("=", TokenType::Assign);
  EQ = Token("==", TokenType::Eq);
  NE = Token("!=", TokenType::Ne);
  INC = Token("++", TokenType::Inc);
  DEC = Token("--", TokenType::Dec);
  ADD = Token("+", TokenType::Add);
  SUB = Token("-", TokenType::Sub);
  MUL = Token("*", TokenType::Mul);
  DIV = Token("/", TokenType::Div);
  LE = Token("<=", TokenType::Le);
  LT = Token("<", TokenType::Lt);
  GE = Token(">=", TokenType::Ge);
  GT = Token(">", TokenType::Gt);
  NOT = Token("!", TokenType::Not);
  LOR = Token("||", TokenType::L_or);
  LAN = Token("&&", TokenType::L_an);
  MOD = Token("%", TokenType::Mod);
  L_BRAK = Token("[", TokenType::L_BRAK);
  R_BRAK = Token("]", TokenType::R_BRAK);
  L_BR = Token("{", TokenType::L_BR);
  R_BR = Token("}", TokenType::R_BR);
  L_PH = Token("(", TokenType::L_PH);
  R_PH = Token(")", TokenType::R_PH);
  COND = Token("?", TokenType::Cond);
  COLON = Token(":", TokenType::Colon);
  S_COLON = Token(";", TokenType::S_Colon);
  COMMA = Token(",", TokenType::Comma);
  PRINTF = Token("printf", TokenType::Printf);
  keywords["int"] = INT;
  keywords["do"] = DO;
  keywords["double"] = DOUBLE;
  keywords["float"] = FLOAT;
  keywords["long"] = LONG;
  keywords["unsigned"] = UNSIGNED;
  keywords["char"] = CHAR;
  keywords["break"] = BREAK;
  keywords["switch"] = SWITCH;
  keywords["if"] = IF;
  keywords["else"] = ELSE;
  keywords["for"] = FOR;
  keywords["while"] = WHILE;
  keywords["enum"] = ENUM;
  keywords["void"] = VOID;
  keywords["main"] = MAIN;
  keywords["return"] = RETURN;
  keywords["sizeof"] = SIZEOF;
  keywords["case"] = CASE;
  keywords["printf"] = PRINTF;
}

Lexer::Lexer(const string &code) : code(code) {
  pos = 0;
  line = code == "" ? 0 : 1;
  init();
  initFunctions();
}

/*
 * Get all the tokens into token list.
 */
void Lexer::lexan() {
  while (pos < code.length()) {
    next();
  }
//  if (pos != 0) {
//    line++;
//  }
}

void Lexer::push(Token *token) {
  curr_token = *token;
  //  cout << "current token " << curr_token << ", at pos " << pos << endl;
  tokens.push_back(token);
}

void Lexer::next() {
  int last_pos;
  int token;
  while (1) {
    if (pos == code.length()) {
      break;
    }
    token = code[pos];
    ++pos;
    if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') ||
        (token == '_')) {
      // find a variable
      last_pos = pos - 1;
      while ((code[pos] >= 'a' && code[pos] <= 'z') ||
             (code[pos] >= 'A' && code[pos] <= 'Z') ||
             (code[pos] >= '0' && code[pos] <= '9') || (code[pos] == '_')) {
        pos++;
      }
      string name = code.substr((size_t)last_pos, (size_t)(pos - last_pos));
      if (keywords.find(name) != keywords.end()) {
        Token *t = &keywords.find(name)->second;
        push(new Token(*t, line));
      } else {
        Token *t = new Token(name, TokenType::Var, line);
        identifiers[name] = *t;
        push(t);
      }
    } else if (token >= '0' && token <= '9') {
      int str = token - '0';
      if (str > 0) {
        while (code[pos] >= '0' && code[pos] <= '9') {
          str = str * 10 + code[pos++] - '0';
        }
      } else {
        if (code[pos] == 'x' || code[pos] == 'X') {
          token = code[++pos];
          while ((token >= '0' && token <= '9') ||
                 (token >= 'a' && token <= 'f') ||
                 (token >= 'A' & token <= 'F')) {
            str = str * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
            token = code[++pos];
          }
        } else {
          while (code[pos] >= '0' && code[pos] <= '7') {
            str = str * 8 + code[pos++] - '0';
          }
        }
      }
      Token *t = new Token(to_string(str), TokenType::Num, line);
      push(t);

    } else if (token == '"' || token == '\'') {
      // TODO: deal with escaped string.
      int cur_pos = pos;
      while (pos < code.length()) {
        if (code[pos] == token) {
          break;
        }
        if (code[pos] == '\\') {
          pos++;
        }
        pos++;
      }
      if (code[pos] != token) {
        pos = cur_pos;
      }
      if (pos - cur_pos == 0) {
        Token *curr_token = new Token("", TokenType::Str, line);
        push(curr_token);
      } else if (pos - cur_pos > 0) {
        string name = code.substr((size_t)cur_pos, (size_t)(pos - cur_pos));
        Token *t = new Token(name, TokenType::Str, line);
        push(t);
      }
      pos++;
    } else if (functions.find(token) != functions.end()) {
      functions.find(token)->second();
    }
  }
}

void Lexer::initFunctions() {
  auto comma = [this]() { push(new Token(COMMA, line)); };
  auto add = [this]() {
    if (code[pos] == '+') {
      pos++;
      push(new Token(INC, line));
    } else {
      push(new Token(ADD, line));
    }
  };
  auto sub = [this]() {
    if (code[pos] == '-') {
      pos++;
      push(new Token(DEC, line));
    } else {
      push(new Token(SUB, line));
    }
  };
  auto eq = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(new Token(EQ, line));
    } else {
      push(new Token(ASSIGN, line));
    }
  };
  auto _not = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(new Token(NE, line));
    } else {
      push(new Token(NOT, line));
    }
  };
  auto gt = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(new Token(GE, line));
    } else {
      push(new Token(GT, line));
    }
  };
  auto div = [this]() {
    if (code[pos] == '/') {
      while (code[pos] != 0 && code[pos] != '\n') {
        ++pos;
      }
      if (code[pos] == '\n') {
        line++;
      }
    } else if (code[pos] == '*') {
      while (pos < code.length() && !(code[pos] == '*' && code[pos + 1] == '/')) {
        if (code[pos] == '\n') {
          line++;
        }
        ++pos;
      }
      pos += 2;
    } else {
      push(new Token(DIV, line));
    }
  };
  auto lt = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(new Token(LE, line));
    } else {
      push(new Token(LT, line));
    }
  };
  auto colon = [this]() { push(new Token(COLON, line)); };
  auto s_colon = [this]() { push(new Token(S_COLON, line)); };
  auto r_ph = [this]() { push(new Token(R_PH, line)); };
  auto l_ph = [this]() { push(new Token(L_PH, line)); };
  auto r_br = [this]() { push(new Token(R_BR, line)); };
  auto l_br = [this]() { push(new Token(L_BR, line)); };
  auto l_brak = [this]() { push(new Token(L_BRAK, line)); };
  auto r_brak = [this]() { push(new Token(R_BRAK, line)); };
  auto cond = [this]() { push(new Token(COND, line)); };
  auto mul = [this]() { push(new Token(MUL, line)); };
  auto mod = [this]() { push(new Token(MOD, line)); };
  auto _and = [this]() {
    if (code[pos] == '&') {
      pos++;
      push(new Token(LAN, line));
    }
  };
  auto _or = [this]() {
    if (code[pos] == '|') {
      pos++;
      push(new Token(LOR, line));
    }
  };
  auto hash = [this]() {
    while (code[pos] != 0 && code[pos] != '\n') {
      pos++;
    }
  };
  auto n = [this]() { line++; };
  functions[','] = comma;
  functions['+'] = add;
  functions['-'] = sub;
  functions['/'] = div;
  functions['*'] = mul;
  functions['='] = eq;
  functions[':'] = colon;
  functions[';'] = s_colon;
  functions['?'] = cond;
  functions['('] = l_ph;
  functions[')'] = r_ph;
  functions['{'] = l_br;
  functions['}'] = r_br;
  functions['['] = l_brak;
  functions[']'] = r_brak;
  functions['%'] = mod;
  functions['&'] = _and;
  functions['|'] = _or;
  functions['>'] = gt;
  functions['<'] = lt;
  functions['!'] = _not;
  functions['\n'] = n;
  functions['#'] = hash;
}
