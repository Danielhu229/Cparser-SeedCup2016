//
// Created by 胡一鸣 on 16/10/27.
//

#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utility.h"

using namespace cParser;
using namespace std;

template <typename ValueType>
ValueType binaryCalculator(Interpreter* interpreter, shared_ptr<Statement> statement) {
  if (statement->token->token == TokenType::Num) {
    return static_cast<ValueType>(atof(statement->token->name.c_str()));
  }if (statement->token->token == TokenType::Var) {
    ValueType value = interpreter->curContext()->get<ValueType>(statement->token->name);
    return value;
  }
  auto right = calculator<ValueType>(interpreter, statement->children[1]);
  auto left = calculator<ValueType>(interpreter, statement->children[0]);
  switch (statement->token->token) {
    case TokenType::Add :
      return left + right;
    case TokenType::Sub :
      return left - right;
    case TokenType::Mul :
      return left * right;
    case TokenType::Div :
      return left / right;
    case TokenType::Assign:
      interpreter->curContext()->set<ValueType>(statement->children[0]->token->name, right);
      interpreter->rSelfOperation();
      return right;
    case TokenType::Lt:
      return static_cast<int>(left < right);
    case TokenType::Gt:
      return static_cast<int>(left > right);
    case TokenType::Le:
      return static_cast<int>(left <= right);
    case TokenType::Ge:
      return static_cast<int>(left >= right);
  }
}

template <typename ValueType>
ValueType lSelfCalculator(Interpreter* interpreter, shared_ptr<Statement> statement) {
  return (static_cast<ValueType>(atof(statement->children[0]->token->name.c_str())) + 1);
}

template <typename ValueType>
ValueType rSelfCalculator(Interpreter* interpreter, shared_ptr<Statement> statement) {
  interpreter->markRSelf(statement->children[0]->token->name, statement->token->token);
  return static_cast<ValueType>(atof(statement->children[0]->token->name.c_str()));
}

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
      return value->second;
    }
    current = current->parent;
  }
  cerr << "undifined var " << name << endl;
  return T(0);
}

template <typename T>
bool Context::has(string name) {
  auto current = this;
  while (current != nullptr) {
    auto value = current->getVars<T>().find(name);
    if (value != getVars<T>().end()) {
      return true;
    }
    current = current->parent;
  }
  return false;
}

Context *Interpreter::curContext() {
  if (this->contexts.empty()) {
    return nullptr;
  }
  return this->contexts.top();
}

void Interpreter::rSelfOperation() {
  for (auto mark : this->marks) {
    if(this->curContext()->has<int>(mark.first)) {
      int val = this->curContext()->get<int>(mark.first);
      this->curContext()->set<int>(mark.first, val + 1);
    }
    else if(this->curContext()->has<float>(mark.first)) {
      float val = this->curContext()->get<float>(mark.first);
      this->curContext()->set<float>(mark.first, val + 1);
    }
    else if(this->curContext()->has<double>(mark.first)) {
      double val = this->curContext()->get<double>(mark.first);
      this->curContext()->set<double>(mark.first, val + 1);

    }
  }
}

void Interpreter::parse(string source) {
  Lexer lexer(source);
  lexer.lexan();
  auto tokens = Utility::combineElseIf(lexer.tokens);
  statements.push_back(Parser::parseTokens(tokens, 0, lexer.tokens.size()));
}
void Interpreter::step() {

}
template<typename T>
T Interpreter::calculate(shared_ptr<Statement> ast) {
  switch (ast->type) {
    case ASTType::Binary:
      return binaryCalculator<T>(this, ast);
    case ASTType::LSelf:
      return lSelfCalculator<T>(this, ast);
  }
}

void Interpreter::markRSelf(string varname, TokenType selfOp) {
  marks[varname] = selfOp;
}
