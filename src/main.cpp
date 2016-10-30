#include <iostream>
#include <memory>
#include <vector>
#include "Parser.h"
#include "Token.h"
#include "io.h"
#include "Interpreter.h"
#include "Utility.h"

using namespace std;
using namespace cParser;


int main(int argc, char const *argv[]) {
  Interpreter interpreter;
  interpreter.build(io::read("input.txt"));
  interpreter.run();
  io::write("output.txt" , Utility::intsToString(interpreter.runLines));
  return 0;
}
