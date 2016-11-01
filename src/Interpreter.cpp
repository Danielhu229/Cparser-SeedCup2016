//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/27.
//

#include "Interpreter.h"
#include "ASTType.h"
#include "Expr.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utility.h"
#include <functional>
#include <map>

using namespace std;

namespace cParser {

string getVarName(Statement *declareOrVar) {
  if (declareOrVar->type == ASTType::DeclareVar) {
    return declareOrVar->children[0]->token.str;
  }
  if (declareOrVar->type == ASTType::Final &&
      declareOrVar->token.type == TokenType::Var) {
    return declareOrVar->token.str;
  }
  cerr << "cannot get var name from ast of" << declareOrVar->token.str << endl;
  return "";
}

template <typename ValueType>
ValueType binaryCalculator(Interpreter *interpreter,
                           const Statement *statement) {
  auto lr = statement->children[0];
  auto rr = statement->children[1];
  auto left = interpreter->calculate<ValueType>(lr);
  auto right = interpreter->calculate<ValueType>(rr);
  ValueType result;
  switch (statement->token.type) {
  case TokenType::Add:
    result = left + right;
    break;
  case TokenType::Sub:
    result = left - right;
    break;
  case TokenType::Mul:
    result = left * right;
    break;
  case TokenType::Div:
    result = left / right;
    break;
  case TokenType::Assign:
    interpreter->curContext()->set<ValueType>(
        getVarName(statement->children[0]), right);
    result = right;
    break;
  case TokenType::Lt:
    result = static_cast<int>(left < right);
    break;
  case TokenType::Gt:
    result = static_cast<int>(left > right);
    break;
  case TokenType::Le:
    result = static_cast<int>(left <= right);
    break;
  case TokenType::Ge:
    result = static_cast<int>(left >= right);
    break;
  case TokenType::Eq:
    result = static_cast<int>(left == right);
    break;
  case TokenType::Ne:
    result = static_cast<int>(left != right);
    break;
  default:
    cerr << "binary parser error, return nullptr" << endl;
    return 0;
  }
  return result;
}

template <typename ValueType>
ValueType commaCalculator(Interpreter *interpreter, Statement *statement) {
  ValueType result;
  for (auto child : statement->children) {
    result = interpreter->calculate<ValueType>(child);
  }

  // comma expression return the last child expression
  return result;
}

template <typename ValueType>
ValueType lSelfCalculator(Interpreter *interpreter, Statement *statement) {
  ValueType result;
  if (statement->token.type == TokenType::Inc) {
    result = interpreter->curContext()->get<ValueType>(
        statement->children[0]->token.str) +
        1;
  } else if (statement->token.type == TokenType::Dec) {
    result = interpreter->curContext()->get<ValueType>(
        statement->children[0]->token.str) -
        1;
  }
  interpreter->curContext()->set<ValueType>(statement->children[0]->token.str,
                                            result);
  return result;
}

void realRSelfCalculator(Interpreter* interpreter, string name, TokenType type) {
  int val = interpreter->curContext()->get<int>(name);
  if (type == TokenType::Inc) {
    interpreter->curContext()->set<int>(name, val + 1);
  } else if (type == TokenType::Dec) {
    interpreter->curContext()->set<int>(name, val - 1);
  }
}

template <typename ValueType>
ValueType rSelfCalculator(Interpreter *interpreter, Statement *statement) {
  // mark first, will be performed later
  interpreter->markRSelf(statement->children[0]->token.str,
                         statement->token.type);
  return static_cast<ValueType>(
      atof(statement->children[0]->token.str.c_str()));
}

template <typename ValueType>
ValueType declareVar(Interpreter *interpreter, Statement *statement) {
  for (auto child : statement->children) {
    if (child->token.type == TokenType::Assign) {
      interpreter->curContext()->declare<ValueType>(
          child->children[0]->token.str,
          interpreter->calculate<ValueType>(child->children[1]));
    } else {
      interpreter->curContext()->declare<ValueType>(child->token.str,
                                                    ValueType(0));
    }
  }
  return ValueType(0);
}

void forExecutor(Interpreter *interpreter, Statement *ast) {
  // initialize operation first
  interpreter->execute(ast->children[0]);
  // transform the calculation of condition
  while (interpreter->calculate<int>(ast->children[1]->children[0])) {
    bool breakFlag(false);
    // should enter a new context
    interpreter->pushContext();

    // execute body
    for (auto child : ast->children[3]->children) {
      // handle
      if (child->type == ASTType::ChildStatement &&
          child->children[0]->token.type == TokenType::Break) {
        breakFlag = true;
        break;
      }

      interpreter->execute(child);
    }

    // should leave the context , all variables are freed
    interpreter->popContext();
    if (breakFlag) {
      break;
    }
    // execute the third statement of for()
    interpreter->execute(ast->children[2]);
  }
}

void whileExecutor(Interpreter *interpreter, Statement *ast) {

  // find the condition
  Statement *condition = ast->children[0];
  while (condition->type == ASTType::ChildStatement) {
    condition = condition->children[0];
  }

  while (interpreter->calculate<int>(condition)) {
    bool breakFlag(false);
    // execute body
    for (auto child : ast->children[1]->children) {
      // break handle
      if (child->type == ASTType::ChildStatement &&
          child->children[0]->token.type == TokenType::Break) {
        interpreter->record(child->token.lineNum);
        breakFlag = true;
        break;
      }
      interpreter->execute(child);
    }
    if (breakFlag) {
      break;
    }
  }
}

// template specific
template <> map<string, int> &Context::getVars<int>() { return this->ints; }

// template specific
template <> map<string, float> &Context::getVars<float>() {
  return this->floats;
}

// template specific
template <> map<string, double> &Context::getVars<double>() {
  return this->doubles;
}

template <class T> void Context::declare(string name, int value) {
  getVars<T>()[name] = value;
}

template <class T> void Context::set(string name, T value) {
  auto current = this;
  // search towards root
  while (current != nullptr) {
    if (current->getVars<T>().find(name) != current->getVars<T>().end()) {
      current->getVars<T>()[name] = value;
      break;
    }
    current = current->parent;
  }
}

template <class T> T Context::get(string name) {
  auto current = this;
  // search towards root
  while (current != nullptr) {
    auto value = current->getVars<T>().find(name);
    if (value != current->getVars<T>().end()) {
      return value->second;
    }
    current = current->parent;
  }
  cerr << "undefined var " << name << endl;
  return T(0);
}

template <typename T> bool Context::has(string name) {
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

void Interpreter::pushContext() {
  this->contexts.push(new Context(this->contexts.top()));
}

void Interpreter::popContext() { this->contexts.pop(); }

void Interpreter::rSelfOperation() {
  for (auto mark : this->marks) {
    if (this->curContext()->has<int>(mark.first)) {
      realRSelfCalculator(this, mark.first, mark.second);
      marks.erase(mark.first);
    } else if (this->curContext()->has<float>(mark.first)) {
      realRSelfCalculator(this, mark.first, mark.second);
      marks.erase(mark.first);
    } else if (this->curContext()->has<double>(mark.first)) {
      realRSelfCalculator(this, mark.first, mark.second);
      marks.erase(mark.first);
    }
  }
}

void Interpreter::step() {
  execute(statements[currentStatement]);
  currentStatement++;
}

void Interpreter::record(int line) {
  if (line == 0) {
    getchar();
  }
  if (!runLines.empty()) {
    if (runLines.back() != line) {
      runLines.push_back(line);
    }
  } else {
    runLines.push_back(line);
  }
}

void Interpreter::execute(Statement *ast) {
  switch (ast->type) {
  case ASTType::Comma:
    calculate<int>(ast);
    break;
  case ASTType::Binary:
    calculate<int>(ast);
    rSelfOperation();
    break;
  case ASTType::LSelf:
    calculate<int>(ast);
    break;
  case ASTType::RSelf:
    calculate<int>(ast);
    rSelfOperation();
    break;
  case ASTType::Final:
    calculate<int>(ast);
    break;
  case ASTType::Call:
    break;
  case ASTType::Printf:
    execute(ast->children[0]);
    rSelfOperation();
    break;
  case ASTType::Block:
    for (auto child : ast->children) {
      execute(child);
    }
    break;
  case ASTType::If:
    pushContext();
    if (calculate<int>(ast->children[0])) {
      execute(ast->children[1]);
    } else {
      if (ast->children.size() == 3 && ast->children[2]) {
        execute(ast->children[2]);
      }
    }
    popContext();
    break;
  case ASTType::Else:
    break;
  case ASTType::ElseIf:
    break;
  case ASTType::For:
    pushContext();
    forExecutor(this, ast);
    popContext();
    break;
  case ASTType::DeclareVar:
    declareVar<int>(this, ast);
    rSelfOperation();
    break;
  case ASTType::ChildStatement:
    if (ast->children.size() > 0) {
      execute(ast->children[0]);
    }
    break;
  case ASTType::While:
    pushContext();
    whileExecutor(this, ast);
    popContext();
    break;
  case ASTType::Do:
    pushContext();
    whileExecutor(this, ast);
    popContext();
    break;
  default:
    break;
  }
}

template <typename T> T Interpreter::calculate(Statement *ast) {
  if (!ast) {
    return T(0);
  }
  switch (ast->type) {
  case ASTType::Comma:
    return commaCalculator<T>(this, ast);
  case ASTType::Binary:
    record(ast->token.lineNum);
    return binaryCalculator<T>(this, ast);
  case ASTType::LSelf:
    record(ast->token.lineNum);
    return lSelfCalculator<T>(this, ast);
  case ASTType::RSelf:
    record(ast->token.lineNum);
    return rSelfCalculator<T>(this, ast);
  case ASTType::Final:
    if (ast->token.type == TokenType::Num) {
      record(ast->token.lineNum);
      auto s = ast->token.str;
      return static_cast<T>(atof(s.c_str()));
    } else if (ast->token.type == TokenType::Var) {
      return curContext()->get<T>(ast->token.str);
    } else if (ast->token.type == TokenType::Str) {
      record(ast->token.lineNum);
      return T(0);
    } else if (ast->token.type == TokenType::S_Colon) {
      return T(0);
    }
      return T(0);
  default:
    cerr << "calculate error" << endl;
    return T(0);
  }
}

void Interpreter::markRSelf(string varname, TokenType selfOp) {
  marks[varname] = selfOp;
}
void Interpreter::run() {
  while (currentStatement < static_cast<int>(statements.size())) {
    step();
  }
}

void Interpreter::build(string source) {
  Lexer lexer(source);
  lexer.lexan();
  Expr expr(lexer.tokens);
  expr.parse();
  this->statements = expr.statements;
}

} // namespace cParser
