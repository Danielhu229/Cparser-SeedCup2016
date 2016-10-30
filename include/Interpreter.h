//
// Created by 胡一鸣 on 16/10/27.
//

#ifndef C_PARSER_INTERPRETER_H
#define C_PARSER_INTERPRETER_H

#include <string>
#include <list>
#include <stack>
#include <utility>
#include <map>
#include <unordered_map>
#include "Token.h"
#include "AST.h"

using namespace std;

namespace cParser {

enum class ContextMode {
  Global,
  If,
  Call,
  Loop
};

class Interpreter;

template <typename ValueType>
ValueType calculator(Interpreter* interpreter, Statement* statement);


class Context {
  Context* parent;
  map<string, int> ints;
  map<string, double> doubles;
  map<string, float> floats;
 public:
  ContextMode mode;
  template <class T>
  map<string, T>& getVars();
  template <class T>
  void declare(string name, int value);
  template <class T>
  void set(string name, T value);
  template <class T>
  T get(string name);
  template <class T>
  bool has(string name);
  Context():parent(nullptr) {}
  Context(Context* parent):parent(parent), mode(ContextMode::Global) {}
  Context(Context* parent, ContextMode mode):parent(parent), mode(mode) {}
};

class Interpreter {
  stack<Context*> contexts;
  int currentStatement;
  void recode(int line);
public:
  vector<int> runLines;
  vector<Statement*> statements;
  unordered_map <string, TokenType> marks;
  void markRSelf(string varname, TokenType selfOp);
  Context* curContext();
  void rSelfOperation();
  void execute(Statement* ast);
  void step();
  string run();
  void build(string source);
  template <typename T>
  T calculate(Statement* ast);
  Interpreter():currentStatement(0) {
    contexts.push(new Context());
  }
};
}

#endif // C_PARSER_INTERPRETER_H
