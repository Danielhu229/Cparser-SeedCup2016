//
// Created by aLIEzTed on 10/27/16.
//

#include "io.h"
#include "gtest/gtest.h"
using namespace cParser;


TEST(shouldReadString, readString) {
  std::string a = io::read("input.txt");
  EXPECT_EQ("Hello World", a);
}