//
// Created by 胡一鸣 on 16/10/26.
//

#include "gtest/gtest.h"
#include "Token.h"
#include "TokenType.h"
#include "Parser.h"

using namespace cParser;

TEST(shouldDivideTokens, binaryParser) {
  auto tokens = new vector<Token *>({
                                        new Token("3435", TokenType::Num),
                                        new Token("+", TokenType::Add),
                                        new Token("78", TokenType::Num)
                                    });
  ParserFun binaryParser = Parser::getUnFinalParser(TokenType::Add);
  auto ast = binaryParser(*tokens, 0, 3, 1);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[0]->token->name, "3435");
  EXPECT_EQ(ast->children[1]->token->name, "78");
}

TEST(shouldTakeBracket, binaryParser) {
  auto tokens = new vector<Token *>({
                                        new Token("(", TokenType::L_BR),
                                        new Token("3435", TokenType::Num),
                                        new Token("+", TokenType::Add),
                                        new Token("78", TokenType::Num),
                                        new Token(")", TokenType::R_BR),
                                        new Token("*", TokenType::Mul),
                                        new Token("57", TokenType::Num)
                                    });
  ParserFun binaryParser = Parser::getUnFinalParser(TokenType::Add);
  auto ast = binaryParser(*tokens, 0, 7, 5);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[0]->token->name, "+");
  EXPECT_EQ(ast->children[1]->token->name, "57");
}
