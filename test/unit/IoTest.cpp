//
// Created by aLIEzTed on 10/27/16.
//

#include "io.h"
#include "gtest/gtest.h"
using namespace cParser;


TEST(file, io) {
  io::write("output.txt", "This is a string");
  std::string a = io::read("output.txt");
  EXPECT_EQ(a,  "This is a string");
}
