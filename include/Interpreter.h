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
#include "Token.h"

using namespace std;

namespace cParser {


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
  Context(Context* parent):parent(parent) {}
};

class Interpreter {
  stack<Context*> contexts;
public:
  Context* curContext();
  void execute(string source);
  Interpreter() {
    contexts.push(new Context(nullptr));
  }
};
}

#endif // C_PARSER_INTERPRETER_H
