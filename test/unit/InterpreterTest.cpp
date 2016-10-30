//
// Created by 胡一鸣 on 16/10/28.
//

#include "gtest/gtest.h"
#include "Interpreter.h"

using namespace cParser;
using namespace std;


TEST(calculate, Statement) {
  std::string a("1 * 8 - 8 + 7;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  auto ast = interpreter->statements[0];
  int result = interpreter->calculate<int>(ast->children[0]);
  EXPECT_EQ(result, 7);
}
TEST(declaration, Statement) {
  std::string a("int i = 24;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 24);
}

TEST(declarationAndCalculate, Statement) {
  std::string a("int i = (4 + 3) * 6 - 10 + 11;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 43);
}

TEST(rselfOperation, Statement) {
  std::string a("int i = 0, j= 0;\nj = i++;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 0);
}

TEST(lselfOperation, Statement) {
  std::string a("int i = 0, j = 0;\nj = ++i;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 1);
}

TEST(continueAssign, Statement) {
  auto interpreter = new Interpreter();
  interpreter->build("int a;\nint b;\na=b=2;");
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("a"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("b"), 2);
}
TEST(continueDeclare, Statement) {
  auto interpreter = new Interpreter();
  interpreter->build("int a, b = 7;\na=2;");
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("a"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("b"), 7);
}