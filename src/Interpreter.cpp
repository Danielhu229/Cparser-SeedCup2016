//
// Created by 胡一鸣 on 16/10/27.
//

#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utility.h"

using namespace cParser;
using namespace std;

string getVarName(shared_ptr<Statement> declareOrVar) {
  if (declareOrVar->type == ASTType::DeclareVar) {
    return declareOrVar->children[0]->token->token_val;
  }
  if (declareOrVar->type == ASTType::Final && declareOrVar->token->token == TokenType::Var) {
    return declareOrVar->token->token_val;
  }
  cerr << "cannot get var name from ast of" << declareOrVar->token->token_val << endl;
  return "";
}

template <typename ValueType>
ValueType binaryCalculator(Interpreter* interpreter, shared_ptr<Statement> statement) {

  auto right = interpreter->calculate<ValueType>(statement->children[1]);
  auto left = interpreter->calculate<ValueType>(statement->children[0]);
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
      getchar();
      interpreter->curContext()->set<ValueType>(getVarName(statement->children[0]), right);
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
  return (static_cast<ValueType>(atof(statement->children[0]->token->token_val.c_str())) + 1);
}

template <typename ValueType>
ValueType rSelfCalculator(Interpreter* interpreter, shared_ptr<Statement> statement) {
  interpreter->markRSelf(statement->children[0]->token->token_val, statement->token->token);
  return static_cast<ValueType>(atof(statement->children[0]->token->token_val.c_str()));
}

template <typename ValueType>
ValueType declareVar(Interpreter* interpreter, shared_ptr<Statement> statement) {
  getchar();
  interpreter->curContext()->declare<ValueType>(statement->children[0]->token->token_val, ValueType(0));
  return ValueType(0);
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
  getchar();
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
  execute(statements[currentStatement]);
}

void Interpreter::execute(shared_ptr<Statement> ast) {
  switch (ast->type) {
    case ASTType::Binary:
      calculate<int>(ast);
      break;
    case ASTType::LSelf:
      calculate<int>(ast);
      break;
    case ASTType::RSelf:
      calculate<int>(ast);
      break;
    case ASTType::Final:
      calculate<int>(ast);
      break;
    case ASTType::Call:break;
    case ASTType::If:break;
    case ASTType::Else:break;
    case ASTType::ElseIf:break;
    case ASTType::For:break;
    case ASTType::DeclareVar:
      calculate<int>(ast);
      break;
    case ASTType::ChildStatement:
      execute(ast->children[0]);
      break;
    case ASTType::While:break;
    case ASTType::Block:break;
  }
}

template<typename T>
T Interpreter::calculate(shared_ptr<Statement> ast) {
  switch (ast->type) {
    case ASTType::Binary:
      return binaryCalculator<T>(this, ast);
    case ASTType::LSelf:
      return lSelfCalculator<T>(this, ast);
    case ASTType::RSelf:
      return rSelfCalculator<T>(this, ast);
    case ASTType::DeclareVar:
      return declareVar<T>(this, ast);
    case ASTType::Final:
      if (ast->token->token == TokenType::Num) {
        return static_cast<T>(atof(ast->token->token_val.c_str()));
      } else if (ast->token->token == TokenType::Var) {
        return curContext()->get<T>(ast->token->token_val);
      }
  }
}

void Interpreter::markRSelf(string varname, TokenType selfOp) {
  marks[varname] = selfOp;
}
