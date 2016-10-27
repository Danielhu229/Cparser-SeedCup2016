//
// Created by 胡一鸣 on 16/10/27.
//

#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utility.h"

using namespace cParser;
using namespace std;

template <> map<string, int> &Context::getVars<int>() { return this->ints; }

template <> map<string, float> &Context::getVars<float>() {
  return this->floats;
}

template <> map<string, double> &Context::getVars<double>() {
  return this->doubles;
}

template <class T> void Context::declare(string name, int value) {
  getVars<T>()[name] = value;
}

template <class T> void Context::set(string name, T value) {
  auto current = this;
  while (current != nullptr) {
    if (current->getVars<T>().find(name) != getVars<T>().end()) {
      current->getVars<T>()[name] = value;
      break;
    }
    current = current->parent;
  }
}
template <class T> T Context::get(string name) {
  auto current = this;
  while (current != nullptr) {
    auto value = current->getVars<T>().find(name);
    if (value != getVars<T>().end()) {
      current->getVars<T>()[name] = value;
      return value;
    }
    current = current->parent;
  }
  cerr << "undifined var " << name << endl;
  return nullptr;
}

Context *Interpreter::curContext() {
  if (this->contexts.empty()) {
    return nullptr;
  }
  return this->contexts.top();
}
void Interpreter::execute(string source) {
  Lexer lexer(source);
  lexer.lexan();
  auto tokens = Utility::combineElseIf(lexer.tokens);
  Parser::parseTokens(lexer.tokens, 0, lexer.tokens.size());
}
