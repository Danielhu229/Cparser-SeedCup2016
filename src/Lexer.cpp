//
// Created by aLIEzTed on 10/24/16.
//

#include "../include/Lexer.h"
using namespace std;

void Lexer::init() {
  INT = Token("int", token_type::Int, "int");
  DOUBLE = Token("double", token_type::Double, "double");
  FLOAT = Token("float", token_type::Float, "float");
  LONG = Token("long", token_type::Long, "long");
  CHAR = Token("char", token_type::Char, "char");
  BREAK = Token("break", token_type::Break, "break");
  SWITCH = Token("switch", token_type::Switch, "switch");
  CASE = Token("case", token_type::Case, "case");
  ELSE = Token("else", token_type::Else, "else");
  ENUM = Token("enum", token_type::Enum, "enum");
  IF = Token("if", token_type::If, "if");
  RETURN = Token("return", token_type::Return, "return");
  SIZEOF = Token("sizeof", token_type::Sizeof, "sizeof");
  WHILE = Token("while", token_type::While, "while");
  FOR = Token("for", token_type::For, "for");
  VOID = Token("void", token_type::Void, "void");
  MAIN = Token("main", token_type::Main, "main");
  UNSIGNED = Token("unsigned", token_type::Unsigned, "unsigned");
  DO = Token("do", token_type::DO, "do");
  ASSIGN = Token("=", token_type::Assign, "=");
  EQ = Token("==", token_type::Eq, "==");
  NE = Token("!=", token_type::Ne, "!=");
  INC = Token("++", token_type::Inc, "++");
  DEC = Token("--", token_type::Dec, "--");
  ADD = Token("+", token_type::Add, "+");
  SUB = Token("-", token_type::Sub, "-");
  MUL = Token("*", token_type::Mul, "*");
  DIV = Token("/", token_type::Div, "/");
  LE = Token("<=", token_type::Le, "<=");
  LT = Token("<", token_type::Lt, "<");
  GE = Token(">=", token_type::Ge, ">=");
  GT = Token(">", token_type::Gt, ">");
  NOT = Token("!", token_type::Not, "!");
  LOR = Token("||", token_type::L_or, "||");
  LAN = Token("&&", token_type::L_an, "&&");
  MOD = Token("%", token_type::Mod, "%");
  L_BRAK = Token("[", token_type::L_BRAK, "[");
  R_BRAK = Token("]", token_type::R_BRAK, "]");
  L_BR = Token("{", token_type::L_BR, "{");
  R_BR = Token("}", token_type::R_BR, "}");
  L_PH = Token("(", token_type::L_PH, "(");
  R_PH = Token(")", token_type::R_PH, ")");
  COND = Token("?", token_type::Cond, "?");
  COLON = Token(":", token_type::Colon, ":");
  S_COLON = Token(";", token_type::S_Colon, ";");
  COMMA = Token(",", token_type::Comma, ",");
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
}

Lexer::Lexer(const string &code) : code(code) {
  pos = 0;
  line = 0;
  init();
  finished = false;
}

/*
 * Get all the tokens into token list.
 */
void Lexer::lexan() {
  int last_pos;
  int token;
  while (pos < code.length()) {
    while (1) {
      if (pos == code.length()) {
        break;
      }
      token = code[pos];
      ++pos;

      if (token == '\n') {
        line++;
      } else if (token == '#') {
        while (code[pos] != 0 && code[pos] != '\n') {
          pos++;
        }
      } else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
        // find a variable
        last_pos = pos - 1;
        while ((code[pos] >= 'a' && code[pos] <= 'z')
            || (code[pos] >= 'A' && code[pos] <= 'Z')
            || (code[pos] >= '0' && code[pos] <= '9')
            || (code[pos] == '_')) {
          pos++;
        }
        string name = code.substr(last_pos, pos - last_pos);
        if (keywords.find(name) != keywords.end()) {
          Token* curr_token = &keywords.find(name)->second;
          push(curr_token);
        } else {
          Token* curr_token = new Token("Var", token_type::Var);
          curr_token->token_val = name;
          identifiers[name] = *curr_token;
          push(curr_token);
        }
        continue;
      } else if (token >= '0' && token <= '9') {
        int token_val = token - '0';
        if (token_val > 0) {
          while (code[pos] >= '0' && code[pos] <= '9') {
            token_val = token_val * 10 + code[pos++] - '0';
          }
        } else {
          if (code[pos] == 'x' || code[pos] == 'X') {
            token = code[++pos];
            while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' & token <= 'F')) {
              token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
              token = code[++pos];
            }
          } else {
            while (code[pos] >= '0' && code[pos] <= '7') {
              token_val = token_val * 8 + code[pos++] - '0';
            }
          }
        }
        Token* curr_token = new Token("Num", token_type::Num);
        curr_token->token_val = to_string(token_val);
        push(curr_token);
        continue;
      } else if (token == '"' || token == '\'') {
        // TODO: deal with escaped string.
        int cur_pos = pos;
        while (pos < code.length()) {
          if (code[pos] == token) {
            break;
          }
          if (code[pos] =='\\') {
            pos++;
          }
          pos++;
        }
        if (code[pos] != token) {
          pos = cur_pos;
          continue;
        }
        if (pos - cur_pos == 1) {
          Token* curr_token = new Token(string(1, code[cur_pos]), token_type::Num);
          curr_token->token_val = string(1, code[cur_pos]);
          push(curr_token);
        } else {
          string name = code.substr(cur_pos, pos - cur_pos);
          Token* curr_token = new Token(name, token_type:: Str);
          curr_token->token_val = name;
          push(curr_token);
        }
        continue;
      } else if (token == '/') {
        if (code[pos] == '/') {
          while (code[pos] != 0 && code[pos] != '\n') {
            ++pos;
          }
        } else {
          Token* curr_token = new Token(DIV);
          push(curr_token);
          continue;
        }
      } else if (token == '=') {
        if (code[pos] == '=') {
          pos++;
          Token* curr_token = new Token(EQ);
          push(curr_token);
        } else {
          Token* curr_token = new Token(ASSIGN);
          push(curr_token);
        }
        continue;
      } else if (token == '+') {
        if (code[pos] == '+') {
          pos++;
          Token* curr_token = new Token(INC);
          push(curr_token);
        } else {
          Token* curr_token = new Token(ADD);
          push(curr_token);
        }
        continue;
      } else if (token == '-') {
        if (code[pos] == '-') {
          pos++;
          Token* curr_token = new Token(DEC);
          push(curr_token);
        } else {
          Token* curr_token = new Token(SUB);
          push(curr_token);
        }
        continue;
      } else if (token == '!') {
        if (code[pos] == '=') {
          pos++;
          Token* curr_token = new Token(NE);
          push(curr_token);
        } else {
          Token* curr_token = new Token(NOT);
          push(curr_token);
        }
        //TODO: judge logical not
        continue;
      } else if (token == '<') {
        if (code[pos] == '=') {
          pos++;
          Token* curr_token = new Token(LE);
          push(curr_token);
        } else {
          Token* curr_token = new Token(LT);
          push(curr_token);
        }
        continue;
      } else if (token == '>') {
        if (code[pos] == '=') {
          pos++;
          Token* curr_token = new Token(GE);
          push(curr_token);
        } else {
          Token* curr_token = new Token(GT);
          push(curr_token);
        }
        continue;
      } else if (token == '|') {
        if (code[pos] == '|') {
          pos++;
          Token* curr_token = new Token(LOR);
          push(curr_token);
        } else {
          cout << "Invalid op" << endl;
        }
        continue;
      } else if (token == '&') {
        if (code[pos] == '&') {
          pos++;
          Token* curr_token = new Token(LAN);
          push(curr_token);
        } else {
          cout << "Invalid op" << endl;
        }
        continue;
      } else if (token == '%') {
        Token* curr_token = new Token(MOD);
        push(curr_token);
        continue;
      } else if (token == '*') {
        Token* curr_token = new Token(MUL);
        push(curr_token);
        continue;
      } else if (token == '[') {
        Token* curr_token = new Token(L_BRAK);
        push(curr_token);
        continue;
      } else if (token == '?') {
        Token* curr_token = new Token(COND);
        push(curr_token);
        continue;
      } else if (token == ']') {
        Token* curr_token = new Token(R_BRAK);
        push(curr_token);
        continue;
      } else if (token == '{') {
        Token* curr_token = new Token(L_BR);
        push(curr_token);
        continue;
      } else if (token == '}') {
        Token* curr_token = new Token(R_BR);
        push(curr_token);
        continue;
      } else if (token == '(') {
        Token* curr_token = new Token(L_PH);
        push(curr_token);
        continue;
      } else if (token == ')') {
        Token* curr_token = new Token(R_PH);
        push(curr_token);
        continue;
      } else if (token == ';') {
        Token* curr_token = new Token(S_COLON);
        push(curr_token);
        continue;
      } else if (token == ':') {
        Token* curr_token = new Token(COLON);
        push(curr_token);
        continue;
      } else if (token == ',') {
        Token* curr_token = new Token(COMMA);
        push(curr_token);
        continue;
      }
    }
  }

}


void Lexer::push(Token* token) {
  tokens.push_back(token);
}
