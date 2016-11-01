//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/28.
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

TEST(calculateOct, Statement) {
  std::string a("1 * 0xA - 8 - 7;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  auto ast = interpreter->statements[0];
  int result = interpreter->calculate<int>(ast->children[0]);
  EXPECT_EQ(result, -5);
}


TEST(calculateFibonacci, Statement) {
  std::string a("int n = 6;printf(\"%d \", __LINE__);\n"
                    "    int result;  printf(\"%d \", __LINE__);\n"
                    "        int previous_result;  printf(\"%d \", __LINE__);\n"
                    "\t    int previous_pre_result;  printf(\"%d \",__LINE__);\n"
                    "\t        result=1;  printf(\"%d \", __LINE__);\n"
                    "\t\t    previous_result=1;  printf(\"%d \", __LINE__);\n"
                    "\t\t        while(printf(\"%d \", __LINE__), n>2)  \n"
                    "\t    {  \n"
                    "\t\t            n--;  printf(\"%d \", __LINE__);\n"
                    "\t\t\t            previous_pre_result=previous_result;printf(\"%d \", __LINE__);\n"
                    "\t\t\t\t            previous_result=result;  printf(\"%d\", __LINE__);\n"
                    "\t\t\t\t\tresult=previous_result+previous_pre_result; printf(\"%d \", __LINE__); \n"
                    "\t\t\t\t\t\t        }  \n"
                    "    printf(\"\", result);  printf(\"%d \", __LINE__);");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("result"), 8);
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

TEST(calculateSigned, Statement) {
  std::string a("int i = -(3+5);");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), -8);
}

// FIXME: signed num;
TEST(calculateSigned2, Statement) {
  std::string a("int i = 3- -5;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 8);
}


TEST(calculateComplex, Statement) {
  std::string a("int a = +2;\n"
                    "int b = a- -1;\n"
                    "\n"
                    "if (a>0)\n"
                    "  for(;a>0;a--)\n"
                    "    b++;\n"
                    "\n"
                    "if(b>0)\n"
                    "  while(b>0)b--;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 4 5 6 5 6 5 8 9");
  EXPECT_EQ(interpreter->curContext()->get<int>("a"), 0);
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

TEST(printfMulti, runControl) {
  std::string a("int pen;\n"
                "int apple;\n"
                "int apple_pen;\n"
                "\n"
                "pen = 1;\n"
                "apple = 2;\n"
                "apple_pen = apple + pen;\n"
                "\n"
                "int pineapple = 4;\n"
                "int pineapple_pen = pineapple + pen;\n"
                "\n"
                "printf(\"I have a pen. %d\\n\", pen);\n"
                "printf(\"I have an apple. %d\\n\", apple);\n"
                "/* PPAP */\n"
                "printf(\"Pen-pineapple-apple pen.\\n\");");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "5 6 7 9 10 12 13 15");
  EXPECT_EQ(interpreter->curContext()->get<int>("pineapple_pen"), 5);
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

// fixme: failed to get the correct result of i, but line number passed
//
TEST(IfWithElseIf, runControl) {
  std::string a("int i, j, k;\nif(i > 1) {\nj--;}\nelse if(i == 0) {\n i++;}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 4 5");
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 1);
}


TEST(IfWithElseIfWithElse, runControl) {
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
  EXPECT_EQ(test, "3 4 5 6 7 10 14 16");
}

TEST(For, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 5; i++) {\n j = 2; \n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 2 3 2 3 2 3 2 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}

TEST(ForWithBreak, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 5; i++) {\n j = 3;break; \n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 3);
}

TEST(ForMoreTimes, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 3; i++) {\n j = 2; \n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 2 3 2 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}

TEST(ForWithComma, runControl) {
  std::string a("int j = 0;\n for(int a = 4, b = 3, c = 5, d = 2; a + b*2 < c + d;"
                    "a = a - b, b++, c--) {\n j = 2; \n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2");
EXPECT_EQ(interpreter->curContext()->get<int>("j"), 0);
}

TEST(ForWithOutBracket, runControl) {
  std::string a("int j = 0;\n for(int i = 0; i < 2; i++)j = 2;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}

TEST(MultiFor, runControl) {
  std::string a("int k = 0;\n for(int i = 0; i < 2; i++){\nfor(int t = 0; t < "
                "3; ++t) \n{k = 2;}\n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 4 3 4 3 4 3 2 3 4 3 4 3 4 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("k"), 2);
}

TEST(MultiFor2, runControl) {
  std::string a("int i, j;\n"
                "\n"
                "for (i = 0; i < 2; i++) {\n"
                "  int i = 2;\n"
                "  for (; i <= 2;) {\n"
                "    i++;j++;\n"
                "  }\n"
                "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "3 4 5 6 5 3 4 5 6 5 3");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 2);
}

TEST(ForIf, runControl) {
  std::string a("int a = 0, b = 0;\n"
                "if (a <= 0) {\n"
                "    for (int i = 0; i != 5; i++) {\n"
                "        b = i;\n"
                "    }\n"
                "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 4 3 4 3 4 3 4 3 4 3");
  EXPECT_EQ(interpreter->curContext()->get<int>("b"), 4);
}

TEST(ForWhile, runControl) {
  std::string a(
      "int k = 0;\n for(int i = 0; i < 2; i++){\nwhile(k == 0) \n{k++;}\n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 4 3 2 3 2");
  EXPECT_EQ(interpreter->curContext()->get<int>("k"), 1);
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

TEST(WhileWithBreak, runControl) {
  std::string a("int j;\n while(j < 2) {\nj++;break;\n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 3");
  EXPECT_EQ(interpreter->curContext()->get<int>("j"), 1);
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

TEST(IfInsideFor, runControl) {
  std::string a(
      "int a = 1;\n for (a = 0; a < 3; a++) {\n if (a == 1) {\na = 4;\n}\n}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
}

TEST(MultiIf, runControl) {
  std::string a(
                    "int a = 0;\n"
                    "int b = 1;\n"
                    "int c = 2;\n"
                    "if (a == 0) {\n"
                    "  if (b == 0) {\n"
                    "    if (c == 2){\n"
                    "      c++;\n"
                    "    }\n"
                    "    else\n"
                    "      c--;\n"
                    "  }\n"
                    "  else {\n"
                    "    if (c == 2)\n"
                    "      c++;\n"
                    "    else\n"
                    "      c--;\n"
                    "  }\n"
                    "}\n"
                    "else {\n"
                    "  a++;\n"
                    "}\n"
                    "\n"
                    "if (a== -1){\n"
                    "  a++;\n"
                    "}\n"
                    "else if (a== 0){\n"
                    "  a++;\n"
                    "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "1 2 3 4 5 13 14 23 26 27");
}

 TEST(WhileInsideForComplex, runControl) {
  std::string a("int i = 2;\n"
                    "int j = 0;\n"
                    "for (; i > 0; i--){\n"
                    "\tprintf(\"hello world!\");\n"
                    "\twhile (j <= 2){\n"
                    "\t\tprintf(\"hello world!\");\n"
                    "\t\tj++;\n"
                    "\t}\n"
                    "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
}

TEST(forEmpty, runControl) {
  std::string a("int a;\n"
                    "int b = 3;\n"
                    "a = 2;\n"
                    "for (;;) {\n"
                    "\tfor (;;) {\n"
                    "\t\ta--;\n"
                    "\t\tif (a <= 0)\n"
                    "\t\t\tbreak;\n"
                    "\t}\n"
                    "\tb--;\n"
                    "\tif (b <= 0)\n"
                    "\t\tbreak;\n"
                    "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 3 4 5 6 7 5 6 7 8 10 11 4 5 6 7 8 10 11 4 5 6 7 8 10 11 12");
}

TEST(whileConst, runControl) {
  std::string a("int a;\n"
                    "a = 2;\n"
                    "while(1)\n"
                    "  if (a-- <= 0)\n"
                    "    break;");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
  EXPECT_EQ(test, "2 3 4 3 4 3 4 5");
}

TEST(LoopComplex, runControl) {
  std::string a("int i = 0, j, k = 0; printf(\"%d \", __LINE__);\n"
                    "\n"
                    "for (int x = 0, y = 0; i < 5; i++) { printf(\"%d \", __LINE__);\n"
                    "  {\n"
                    "    while (1) { printf(\"%d \", __LINE__);\n"
                    "      y = 1 + y; printf(\"%d \", __LINE__);\n"
                    "      printf(\"%d \", __LINE__); if (y < 3) {\n"
                    "        while (1) { printf(\"%d \", __LINE__);\n"
                    "          j = k++; printf(\"%d \", __LINE__);\n"
                    "          {\n"
                    "            printf(\"%d \", __LINE__); break;\n"
                    "          }\n"
                    "        }\n"
                    "      }\n"
                    "      else {\n"
                    "        printf(\"%d \", __LINE__); break;\n"
                    "      }\n"
                    "    }\n"
                    "    printf(\"%d \", __LINE__); if (i == 4) {\n"
                    "      printf(\"%d \", __LINE__);break;\n"
                    "    }\n"
                    "  }\n"
                    "}");
  auto interpreter = new Interpreter();
  interpreter->build(a);
  interpreter->run();
  auto test = Utility::intsToString(interpreter->runLines);
}

