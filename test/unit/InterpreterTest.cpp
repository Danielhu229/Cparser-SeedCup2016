//
// Created by 胡一鸣 on 16/10/28.
//

#include "gtest/gtest.h"
#include "Interpreter.h"

using namespace cParser;
using namespace std;



TEST(calculate, Statement) {
  std::string a("1 * 8 - 8 + 7");
  auto interpreter = new Interpreter();
  interpreter->parse(a);
  auto ast = interpreter->statements[0];
  int result = interpreter->calculate<int>(ast);
  EXPECT_EQ(result, 7);
}

TEST(declaration, Statement) {
  std::string a("int i = 24;");
  auto interpreter = new Interpreter();
  interpreter->parse(a);
  interpreter->step();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 24);
}

TEST(declarationAndCalculate, Statement) {
  std::string a("int i = (4 + 3) * 6 - 10 + 11;");
  auto interpreter = new Interpreter();
  interpreter->parse(a);
  interpreter->step();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 43);
}

TEST(selfOperation, Statement) {
  std::string a("int i;");
  std::string b("i++;");
  auto interpreter = new Interpreter();
  interpreter->parse(a);
  interpreter->parse(b);
  interpreter->step();
  interpreter->step();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
}


TEST(continueAssign, Statement) {
  auto interpreter = new Interpreter();
  interpreter->parse("int a;");
  interpreter->parse("int b;");
  interpreter->parse("a=b=2;");
  interpreter->step();
  interpreter->step();
  interpreter->step();
  EXPECT_EQ(interpreter->curContext()->get<int>("a"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("b"), 2);
}