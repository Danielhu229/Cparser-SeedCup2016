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


enum class ValueType {
  Int,
  Float,
  Double,
  Void
};

class Interpreter;

template <typename ValueType>
ValueType calculator(Interpreter* interpreter, shared_ptr<Statement> statement);


class Context {
  Context* parent;
  map<string, int> ints;
  map<string, double> doubles;
  map<string, float> floats;
 public:
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
  Context(Context* parent):parent(parent) {}
};

class Interpreter {
  stack<Context*> contexts;
  vector<shared_ptr<Statement>> statements;
public:
  unordered_map <string, TokenType> marks;
  void markRSelf(string varname, TokenType selfOp);
  Context* curContext();
  void rSelfOperation();
  void parse(string source);
  void step();
  template <typename T>
  T calculate(shared_ptr<Statement> ast);
  Interpreter() {
    contexts.push(new Context(nullptr));
  }
};
}

#endif // C_PARSER_INTERPRETER_H
