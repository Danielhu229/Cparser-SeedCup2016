#include "io.h"
#include <iostream>

using namespace std;
using namespace cParser;

int main(int argc, char const *argv[]) {
  io::write("output.txt", io::read("input.txt"));
  return 0;
}
