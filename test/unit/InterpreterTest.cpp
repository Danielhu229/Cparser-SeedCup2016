//
// Created by 胡一鸣 on 16/10/28.
//

#include "gtest/gtest.h"
#include "Interpreter.h"

using namespace cParser;
using namespace std;

TEST(declaration, Statement) {
  auto interpreter = new Interpreter();
  interpreter->parse("int i = 24;");
  interpreter->step();
  EXPECT_EQ(interpreter->curContext()->get<int>("i"), 24);
}