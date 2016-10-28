#include "Parser.h"
#include "Token.h"
#include "io.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace cParser;

void printAST(shared_ptr<Statement> ast) {
    cout << ast->token->name << " ";
    for (auto childAST : ast->children) {
      printAST(childAST);
    }
}

void manualTest1() {
  auto tokens = new vector<Token *>({new Token("(", TokenType::L_PH),
                                     new Token("3435", TokenType::Num),
                                     new Token("+", TokenType::Add),
                                     new Token("78", TokenType::Num),
                                     new Token(")", TokenType::L_PH),
                                     new Token("*", TokenType::Mul),
                                     new Token("57", TokenType::Num)
                                    });

  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  printAST(ast);
}

void manualTest2() {
  auto tokens = new vector<Token *>({new Token("(", TokenType::L_PH),
                                     new Token("3435", TokenType::Num),
                                     new Token("+", TokenType::Add),
                                     new Token("78", TokenType::Num),
                                     new Token(")", TokenType::R_PH),
                                     new Token("*", TokenType::Mul),
                                     new Token("57", TokenType::Num)
                                    });

  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  printAST(ast);
}

int main(int argc, char const *argv[]) {
  manualTest1();
  return 0;
}
