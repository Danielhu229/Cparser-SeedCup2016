//
// Created by 胡一鸣 on 16/10/27.
//

#ifndef C_PARSER_INTERPRETER_H
#define C_PARSER_INTERPRETER_H

#include <string>

using namespace std;

namespace cParser {

class Interpreter {
public:
  void execute(string source);
};
}

#endif // C_PARSER_INTERPRETER_H
