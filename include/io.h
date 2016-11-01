//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/24.
//

#ifndef C_PARSER_IO_H
#define C_PARSER_IO_H

#include <fstream>
#include <iostream>
#include <string>

constexpr int bufferSize = 10;

using namespace std;

namespace cParser {
namespace io {
char result[1000];

inline string read(const string &relativePath) {
  ifstream f(relativePath);
  f.read(result, 1000);
  f.close();
  return result;
}

inline void write(const string &relativePath, const string &content) {
  ofstream f(relativePath);
  if (f.is_open()) {
    f << content;
    f.close();
  } else {
    cerr << "cannot open file" << endl;
  }
  return;
}
} // namespace cParser::io
}; // namespace cParser::io

#endif // C_PARSER_IO_H
