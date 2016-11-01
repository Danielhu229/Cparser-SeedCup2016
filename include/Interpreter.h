//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/27.
//

#ifndef C_PARSER_INTERPRETER_H
#define C_PARSER_INTERPRETER_H

#include "AST.h"
#include "Token.h"
#include <functional>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

namespace cParser {

class Interpreter;

template <typename ValueType>
ValueType calculator(Interpreter *interpreter, Statement *statement);

/**
 * variable context
 */
class Context {

  Context *parent;

  map<string, int> ints;

  map<string, double> doubles;

  map<string, float> floats;

public:
  /**
   * get current var list for specific type
   * @return map<string, T> class by class parameter
   */
  template <class T> map<string, T> &getVars();

  /**
   * declare variable in this context
   */
  template <class T> void declare(string name, int value);

  /**
   * set variable (start search from this context to root)
   */
  template <class T> void set(string name, T value);

  /**
   * get variable value (start search from this context to root)
   */
  template <class T> T get(string name);

  /**
   * check if specific var has been declared
   */
  template <class T> bool has(string name);

  Context() : parent(nullptr) {}

  /**
   * construct with parent context
   */
  Context(Context *parent) : parent(parent) {}
};

class Interpreter {

  /**
   * context stack, maintainning all context
   */
  stack<Context *> contexts;

  int currentStatement;

  void step();

public:
  vector<int> runLines;

  vector<Statement *> statements;

  unordered_map<string, TokenType> marks;

  /**
   * r self operator marker, will be performed after calculate
   */
  void markRSelf(string varname, TokenType selfOp);

  void record(int line);

  Context *curContext();

  void pushContext();

  void popContext();

  /**
   * find all r-self (like i++) operation
   */
  void rSelfOperation();

  void execute(Statement *ast);

  /**
   * start interpret
   */
  void run();

  /**
   * build the abstract syntax trees
   */
  void build(string source);

  template <typename T> T calculate(Statement *ast);

  Interpreter() : currentStatement(0) { contexts.push(new Context()); }
};
} // namespace cParser

#endif // C_PARSER_INTERPRETER_H
