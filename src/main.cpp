#include "io.h"
#include <iostream>

using namespace std;
using namespace cParser;

int main(int argc, char const *argv[]) {
  io::write("test_write", io::read("test"));
  return 0;
}
