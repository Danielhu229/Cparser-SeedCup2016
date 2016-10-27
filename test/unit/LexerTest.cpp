//
// Created by aLIEzTed on 10/26/16.
//
#include "Lexer.h"
#include "gtest/gtest.h"


TEST(shouldFindParentheses, findParenttheses) {
  std::string a("()");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 2);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::L_PH);
  EXPECT_EQ(lexer.tokens[1]->token, TokenType::R_PH);
}

TEST(shouldFindBrackets, findBrackets) {
  std::string a("a[0]");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 4);
  EXPECT_EQ(lexer.tokens[1]->token, TokenType::L_BRAK);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::R_BRAK);
}

TEST(shouldFindBraces, findBraces) {
  std::string a("test() { }");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 5);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::L_BR);
  EXPECT_EQ(lexer.tokens[4]->token, TokenType::R_BR);
}

TEST(shouldFindComma, findComma) {
  std::string a("int a, b;");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 5);
  EXPECT_EQ(lexer.tokens[2]->token, TokenType::Comma);
}

TEST(shouldFindColon, findColon) {
  std::string a("for (int i : x) i++;");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 10);
  EXPECT_EQ(lexer.tokens[4]->token, TokenType::Colon);
}

TEST(shouldFindSemicolon, findSemicolon) {
  std::string a(";");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 1);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::S_Colon);
}

TEST(shouldFindLogicalAnd, findLogicalAnd) {
  std::string a("if (a && b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::L_an);
}

TEST(shouldFindGt, findGt) {
  std::string a("if (a > b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Gt);
}

TEST(shouldFindGe, findGe) {
  std::string a("if (a >= b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Ge);
}

TEST(shouldFindLt, findLt) {
  std::string a("if (a < b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Lt);
}

TEST(shouldFindLe, findLe) {
  std::string a("if (a <= b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Le);
}


TEST(shouldFindLogicalOr, findLogicalOr) {
  std::string a("if (a || b)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::L_or);
}


TEST(shouldFindNot, findNot) {
  std::string a("if (!a)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 5);
  EXPECT_EQ(lexer.tokens[2]->token, TokenType::Not);
}

TEST(shouldFindNotEq, findNotEq) {
  std::string a("if (a != 0)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Ne);
}

TEST(shouldFindEq, findEq) {
  std::string a("if (a == 0)");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 6);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Eq);
}

TEST(shouldFindMod, findMod) {
  std::string a("int a = a % 3;");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 7);
  EXPECT_EQ(lexer.tokens[4]->token, TokenType::Mod);
}

TEST(shouldFindCommonModInStr, findCommonMod) {
  std::string a("\"%d\"");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 1);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Str);
  EXPECT_EQ(lexer.tokens[0]->token_val, "%d");
}

TEST(shouldFindReturn, findReturn) {
  std::string a("return 0");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 2);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Return);
}


TEST(shouldFindComment, findComment) {
  std::string a(" //This is a comment.");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 0);
}


TEST(shouldFindLongComment, findLongComment) {
  std::string a("/*\n*This is a big comment\n*/");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 0);
}

TEST(shouldFindTwoVar, findTwoVar) {
  std::string a("int a, b");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 4);
  EXPECT_EQ(lexer.identifiers.size(), 2);
  EXPECT_EQ(lexer.tokens[1]->token, TokenType::Var);
  EXPECT_EQ(lexer.tokens[1]->token_val, "a");
  EXPECT_EQ(lexer.tokens[3]->token_val, "b");
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::Var);

}
TEST(shouldFindKeywords, findKeywords) {
  std::string a("void main");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 2);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Void);
  EXPECT_EQ(lexer.tokens[1]->token, TokenType::Main);
}

TEST(shouldCountTwoLine, countTwoLine) {
  std::string a("void \nmain");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 2);
  EXPECT_EQ(lexer.line, 2);
}

TEST(shouldCountOneLineInComment, countOneLineInComment) {
  std::string a("//This is a comment");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 0);
  EXPECT_EQ(lexer.line, 1);
}

TEST(shouldCountNoLine, countNoLine) {
  std::string a("");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 0);
  EXPECT_EQ(lexer.line, 0);
}

TEST(shouldCountThreeInComment, countThreeLine) {
  std::string a("/*\n*This is a big comment\n*/");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 0);
  EXPECT_EQ(lexer.line, 3);
}

TEST(shouldFindWeirdVar, findWeirdVar) {
  std::string a("whileforvoid");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 1);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Var);
  EXPECT_EQ(lexer.tokens[0]->token_val, "whileforvoid");
}

TEST(shouldFindInc, findInc) {
  std::string a("int a++;");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 4);
  EXPECT_EQ(lexer.tokens[2]->token, TokenType::Inc);
}

TEST(shouldFindDec, findDec) {
  std::string a("int b--;");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 4);
  EXPECT_EQ(lexer.tokens[2]->token, TokenType::Dec);
}

TEST(shouldFindWhile, findWhile) {
  std::string a("while (a < 3) { a++; }");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 11);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::While);
}

TEST(shouldFindNum, findNum) {
  std::string a("1 + 1 = 2");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 5);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Num);
  EXPECT_EQ(lexer.tokens[0]->token_val, "1");
}

TEST(shouldFindString, findString) {
  std::string a("\"Hello World\"");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 1);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Str);
  EXPECT_EQ(lexer.tokens[0]->token_val, "Hello World");
}

TEST(shouldFindAllTokens, findAllTokens) {
  std::string a("#include <stdio.h> \n void main() { printf(\"hello world\"); } ");
  Lexer lexer(a);
  lexer.lexan();
  EXPECT_EQ(lexer.tokens.size(), 11);
  EXPECT_EQ(lexer.tokens[0]->token, TokenType::Void);
  EXPECT_EQ(lexer.tokens[1]->token, TokenType::Main);
  EXPECT_EQ(lexer.tokens[2]->token, TokenType::L_PH);
  EXPECT_EQ(lexer.tokens[3]->token, TokenType::R_PH);
  EXPECT_EQ(lexer.tokens[4]->token, TokenType::L_BR);
  EXPECT_EQ(lexer.tokens[5]->token, TokenType::Var);
  EXPECT_EQ(lexer.tokens[5]->token_val, "printf");
  EXPECT_EQ(lexer.tokens[6]->token, TokenType::L_PH);
  EXPECT_EQ(lexer.tokens[7]->token, TokenType::Str);
  EXPECT_EQ(lexer.tokens[7]->token_val, "hello world");
  EXPECT_EQ(lexer.tokens[8]->token, TokenType::R_PH);
  EXPECT_EQ(lexer.tokens[9]->token, TokenType::S_Colon);
  EXPECT_EQ(lexer.tokens[10]->token, TokenType::R_BR);
  EXPECT_EQ(lexer.line, 2);
}










