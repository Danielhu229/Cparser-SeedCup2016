//
// Created by 胡一鸣 on 16/10/28.
//

#include "Interpreter.h"
#include "Utility.h"
#include "gtest/gtest.h"

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

TEST(declarationWithCommnt, Statement) {
  std::string a("/* This is a Comment */ \nint i = 24;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 24);
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2");
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

TEST(printf, runControl) {
  std::string a("int j;\n printf(\"test%d\", j++);");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 1);
}

TEST(sequence, runControl) {
  std::string a("int i, j, k;\ni = 2;\nj = 3;\nk = 4;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 3 4");
}

TEST(If, runControl) {
  std::string a("int i, j, k;\nif(i > 1) {\nj--;}\nelse {\n i++;}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 5");
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
}


TEST(IfWithoutBracket, runControl) {
  std::string a("int i, j, k;\nif(i > 1) j--;else i++;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2");
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
}


TEST(IfWithElseIf, runControl) {
  std::string a("int i, j, k;\nif(i > 1) {\nj--;}\nelse if(i == 0) {\n i++;}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 4 5");
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 0);
}

TEST(multi, runControl) {
  std::string a("int apple;\n"
                    "int orange;\n"
                    "orange = 2;\n"
                    "apple = 1;\n"
                    "apple = apple + 1;\n"
                    "orange = apple + orange;\n"
                    "if (orange == 2){\n"
                    "\tprintf(\"%d\", apple);\n"
                    "}\n"
                    "else if(orange == 3){\n"
                    "\tprintf(\"%d\", apple);\n"
                    "}\n"
                    "else{\n"
                    "\tprintf(\"%d\", orange);\n"
                    "}\n"
                    "printf(\"%d\", orange);");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(interpreter->curContext()->get<int>("apple"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("orange"), 4);
}



TEST(For, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 5; i++) {\n j = 2; \n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 2 3 2 3 2 3 2 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 0);
}

//TEST(ForMoreTimes, runControl) {
//  std::string a("int j = 0;\n for(int i = 0; i < 3; i++) {\n j = 2; \n}");
//  auto interpreter = new Interpreter();
//  interpreter->build(a);
//  interpreter->run();
//  auto test = Utility::intsToString(interpreter->runLines);
//  EXPECT_EQ(test, "1 2 3 2 3 2 3 2 3 2");
//  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
//  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 0);
//}

TEST(ForWithOutBracket, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 2; i++)j = 2;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 0);
}


TEST(While, runControl) {
  std::string a("int j;\n while(j < 2) {\nj++;\n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 3 2 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}

TEST(WhileWithoutBracket, runControl) {
  std::string a("int j;\n while(j < 2) j++;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}


TEST(DoWhile, runControl) {
  std::string a("int j;\n do j++; while (j < 2);");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}
