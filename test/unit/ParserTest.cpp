//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/26.
//

#include "Parser.h"
#include "Token.h"
#include "TokenType.h"
#include "Utility.h"
#include "gtest/gtest.h"
using namespace cParser;

TEST(shouldDivideTokens, binaryParser) {
  auto tokens = new vector<Token *>({new Token("3435", TokenType::Num),
                                     new Token("+", TokenType::Add),
                                     new Token("78", TokenType::Num)});
  ParserFun binaryParser = Parser::getUnFinalParser(TokenType::Add);
  auto ast = binaryParser(*tokens, 0, 3, 1);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[0]->token.str, "3435");
  EXPECT_EQ(ast->children[1]->token.str, "78");
}

TEST(shouldTakeNegative, parseTokens) {
  auto tokens = new vector<Token *>(
      {new Token("a", TokenType::Var), new Token("=", TokenType::Eq),
       new Token("-", TokenType::Sub), new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  EXPECT_EQ(ast->type, ASTType::Binary);
}

TEST(shouldTakeBracket, binaryParser) {
  auto tokens = new vector<Token *>(
      {new Token("(", TokenType::L_PH), new Token("3435", TokenType::Num),
       new Token("+", TokenType::Add), new Token("78", TokenType::Num),
       new Token(")", TokenType::R_PH), new Token("*", TokenType::Mul),
       new Token("57", TokenType::Num)});
  ParserFun binaryParser = Parser::getUnFinalParser(TokenType::Mul);
  auto ast = binaryParser(*tokens, 0, 7, 5);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->token.str, "*");
  EXPECT_EQ(ast->children[0]->token.str, "+");
  EXPECT_EQ(ast->children[1]->token.str, "57");
}

TEST(shouldParseLeft, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("++", TokenType::Inc), new Token("i", TokenType::Var)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, tokens->size(), 0);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token.str, "i");
}

TEST(shouldParseRight, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("i", TokenType::Var), new Token("--", TokenType::Dec)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, tokens->size(), 1);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token.str, "i");
}

TEST(shouldParseComplex, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token("+", TokenType::Add), new Token("a", TokenType::Var),
       new Token("--", TokenType::Dec)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, 2, 1);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token.str, "i");

  ast = selfPerser(*tokens, 3, 5, 4);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token.str, "a");
}

TEST(shouldParseComplex, printfParser) {
  auto tokens = new vector<Token *>(
      {new Token("printf", TokenType::Printf), new Token("(", TokenType::L_PH),
       new Token("\"hello world%d\"", TokenType::Str),
       new Token(",", TokenType::Comma), new Token("7", TokenType::Num),
       new Token(")", TokenType::R_PH)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Printf);
  EXPECT_EQ(ast->type, ASTType::Printf);
}

TEST(shouldParseGt, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token(">", TokenType::Gt),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Gt);
}

TEST(shouldParseGe, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token(">=", TokenType::Ge),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Ge);
}

TEST(shouldParseLt, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token("<", TokenType::Lt),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Lt);
}

TEST(shouldParseLe, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token("<=", TokenType::Le),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Le);
}

TEST(shouldParseEq, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token("==", TokenType::Eq),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Eq);
}

TEST(shouldParseComma, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("1", TokenType::Int), new Token(",", TokenType::Comma),
       new Token("2", TokenType::Int), new Token(",", TokenType::Comma),
       new Token("3", TokenType::Int)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Comma);
  auto ast = selfPerser(*tokens, 0, tokens->size(), 1);
  EXPECT_EQ(ast->children.size(), 3);
  EXPECT_EQ(ast->token.type, TokenType::Comma);
}

TEST(shouldParseNe, printfParser) {
  auto tokens = new vector<Token *>({new Token("1", TokenType::Num),
                                     new Token("!=", TokenType::Ne),
                                     new Token("1", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, 3);
  EXPECT_EQ(ast->token.type, TokenType::Ne);
}

TEST(shouldParseBlock, blockParser) {
  auto tokens = new vector<Token *>({
      new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
      new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
      new Token("a", TokenType::Var), new Token("--", TokenType::Dec),
      new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
  });
  ParserFun blockParser = Parser::getUnFinalParser(TokenType::L_BR);
  auto ast = blockParser(*tokens, 0, (int)tokens->size(), 0);
  EXPECT_EQ(ast->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children.size(), 2);
}

TEST(shouldParseBlockInBlock, blockParser) {
  auto tokens = new vector<Token *>({
      new Token("{", TokenType::L_BR, 1), new Token("{", TokenType::L_BR, 2),
      new Token(";", TokenType::S_Colon, 3), new Token("}", TokenType::R_BR, 4),
      new Token("}", TokenType::R_BR, 5),
  });
  ParserFun blockParser = Parser::getUnFinalParser(TokenType::L_BR);
  auto ast = blockParser(*tokens, 0, (int)tokens->size(), 0);
  EXPECT_EQ(ast->token.type, TokenType::L_BR);
}

TEST(shouldParseEmptyBlock, blockParser) {
  auto tokens = new vector<Token *>({
      new Token("{", TokenType::L_BR), new Token("}", TokenType::R_BR),
  });
  ParserFun blockParser = Parser::getUnFinalParser(TokenType::L_BR);
  auto ast = blockParser(*tokens, 0, (int)tokens->size(), 0);
  EXPECT_EQ(ast->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children.size(), 0);
}
TEST(shouldParseExpression, Parse) {
  auto tokens = new vector<Token *>(
      {new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token("+", TokenType::Add), new Token("a", TokenType::Var),
       new Token("--", TokenType::Dec), new Token("*", TokenType::Mul),
       new Token("4", TokenType::Num)});
  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  EXPECT_EQ(ast->type, ASTType::Binary);
  EXPECT_EQ(ast->token.type, TokenType::Add);
}

TEST(shouldParseIfExpression, ifParser) {
  auto tokens = new vector<Token *>({
      new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
      new Token("a", TokenType::Var), new Token(")", TokenType::R_PH),
      new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
      new Token("=", TokenType::Assign), new Token("4", TokenType::Num),
      new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
  });
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
}

TEST(shouldParseIfExpressionCond, ifParser) {
  auto tokens = new vector<Token *>({
      new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
      new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
      new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
      new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
      new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
      new Token("}", TokenType::R_BR),
  });
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseIfElseExpression, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If),     new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),     new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num),     new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR),    new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc),    new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR),    new Token("else", TokenType::Else),
       new Token("{", TokenType::L_BR),    new Token("j", TokenType::Var),
       new Token("--", TokenType::Dec),    new Token(";", TokenType::S_Colon),
       new Token("k", TokenType::Var),     new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseIfNoBracketsElse, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
       new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
       new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseIfElseNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
      new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
      new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
      new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
      new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
      new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
      new Token("h", TokenType::Var), new Token("--", TokenType::Dec),
      new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
      new Token("else", TokenType::Else), new Token("j", TokenType::Var),
      new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
  });
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Dec);
}

TEST(shouldParserIfElseIf, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If),     new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),     new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num),     new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR),    new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc),    new Token(";", TokenType::S_Colon),
       new Token("j", TokenType::Var),     new Token("--", TokenType::Dec),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
       new Token("else", TokenType::Else), new Token("if", TokenType::If),
       new Token("(", TokenType::L_PH),    new Token("a", TokenType::Var),
       new Token("==", TokenType::Eq),     new Token("0", TokenType::Num),
       new Token(")", TokenType::R_PH),    new Token("{", TokenType::L_BR),
       new Token("j", TokenType::Var),     new Token("--", TokenType::Dec),
       new Token(";", TokenType::S_Colon), new Token("h", TokenType::Var),
       new Token("++", TokenType::Inc),    new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR),    new Token("else", TokenType::Else),
       new Token("{", TokenType::L_BR),    new Token("h", TokenType::Var),
       new Token("--", TokenType::Dec),    new Token(";", TokenType::S_Colon),
       new Token("k", TokenType::Var),     new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2]->children[2]->token.type, TokenType::L_BR);
}

TEST(shouldParserIfElseIfNoneBrackets, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
       new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("j", TokenType::Var), new Token("--", TokenType::Dec),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::Dec);
}

TEST(shouldParserIfElseIfNoBrackets, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If),    new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),    new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num),    new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR),   new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc),   new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR),   new Token("else", TokenType::Else),
       new Token("if", TokenType::If),    new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),    new Token("==", TokenType::Eq),
       new Token("0", TokenType::Num),    new Token(")", TokenType::R_PH),
       new Token("j", TokenType::Var),    new Token("--", TokenType::Dec),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::Dec);
}

TEST(shouldParseIfNoBracketsElseIf, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If),     new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),     new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num),     new Token(")", TokenType::R_PH),
       new Token("i", TokenType::Var),     new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
       new Token("if", TokenType::If),     new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),     new Token("==", TokenType::Eq),
       new Token("0", TokenType::Num),     new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR),    new Token("j", TokenType::Var),
       new Token("--", TokenType::Dec),    new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token.type,
            TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->children[0]->token.type,
            TokenType::Dec);
}

TEST(shouldParseIfElseEitherNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
      new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
      new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
      new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
      new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
      new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
      new Token("j", TokenType::Var), new Token("--", TokenType::Dec),
      new Token(";", TokenType::S_Colon),
  });
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Dec);
}

TEST(shouldParseIfExpressionComplex, ifParser) {
  auto tokens = new vector<Token *>({
      new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
      new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
      new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
      new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
      new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
      new Token("j", TokenType::Var), new Token("--", TokenType::Dec),
      new Token(";", TokenType::S_Colon),

      new Token("{", TokenType::L_BR), new Token("}", TokenType::R_BR),

      new Token("}", TokenType::R_BR),
  });
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children.size(), 3);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[2]->children.size(), 0);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[2], nullptr);
}

TEST(shouldParseIfNoBrackets, ifParser) {
  auto tokens = new vector<Token *>(
      {new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token.type, TokenType::If);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[2], nullptr);
}

TEST(shouldParseWhileExpr, whileParser) {
  auto tokens = new vector<Token *>(
      {new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token.type, TokenType::While);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseWhileExprInBlock, whileParser) {
  auto tokens = new vector<Token *>(
      {new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("{", TokenType::L_BR), new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
       new Token("j", TokenType::Var), new Token("--", TokenType::Dec),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token.type, TokenType::While);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token.type,
            TokenType::Dec);
  EXPECT_EQ(ast->children[1]->children[1]->token.type, TokenType::S_Colon);
}

TEST(shouldParseWhileNoBrackets, whileParser) {
  auto tokens = new vector<Token *>(
      {new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
       new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token.type, TokenType::While);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
}

TEST(shouldParseForExpr, forParser) {
  auto tokens = new vector<Token *>(
      {new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
       new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
       new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
       new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token.type, TokenType::For);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[3]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->children[0]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseForExprInBlock, forParser) {
  auto tokens = new vector<Token *>(
      {new Token("for", TokenType::For),   new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var),     new Token("=", TokenType::Assign),
       new Token("0", TokenType::Num),     new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var),     new Token("<", TokenType::Lt),
       new Token("10", TokenType::Num),    new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var),     new Token("++", TokenType::Inc),
       new Token(")", TokenType::R_PH),    new Token("{", TokenType::L_BR),
       new Token("i", TokenType::Var),     new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("a", TokenType::Var),
       new Token("=", TokenType::Assign),  new Token("1", TokenType::Num),
       new Token("+", TokenType::Add),     new Token("2", TokenType::Num),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token.type, TokenType::For);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->token.type, TokenType::L_BR);
  EXPECT_EQ(ast->children[3]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->children[0]->children[0]->token.type,
            TokenType::Inc);
  EXPECT_EQ(ast->children[3]->children[1]->children[0]->token.type,
            TokenType::Assign);
}

TEST(shouldParseForExprNoBrackets, forParser) {
  auto tokens = new vector<Token *>(
      {new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
       new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
       new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
       new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(")", TokenType::R_PH), new Token("i", TokenType::Var),
       new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token.type, TokenType::For);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(ast->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(ast->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->children[0]->token.type, TokenType::Inc);
}

TEST(shouldParseSimiColon, Parse) {
  auto tokens = new vector<Token *>(
      {new Token("4", TokenType::Num), new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  EXPECT_EQ(ast->type, ASTType::ChildStatement);
  EXPECT_EQ(ast->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Num);
}

TEST(shouldFindLastPosOfBrackets, findBr) {
  auto tokens = new vector<Token *>(
      {new Token("{", TokenType::L_BR), new Token("{", TokenType::L_BR),
       new Token("}", TokenType::R_BR), new Token("}", TokenType::R_BR)});
  auto ast = Utility::findBr(*tokens, 0, tokens->size());
  EXPECT_EQ(ast, 3);
}

TEST(shouldParseDoWhileExpr, dowhileParser) {
  auto tokens = new vector<Token *>(
      {new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
       new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
       new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
       new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
       new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::Do);
  EXPECT_EQ(ast->token.type, TokenType::DO);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token.type, TokenType::Ne);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token.type,
            TokenType::Inc);
}

TEST(shouldParseDoWhileNoBracketsExpr, dowhileParser) {
  auto tokens = new vector<Token *>(
      {new Token("do", TokenType::DO), new Token("a", TokenType::Var),
       new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
       new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
       new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
       new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
       new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::Do);
  EXPECT_EQ(ast->token.type, TokenType::DO);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token.type, TokenType::Ne);
  EXPECT_EQ(ast->children[1]->children[0]->token.type, TokenType::Inc);
}

TEST(shouldParseBreak, parse) {
  auto tokens = new vector<Token *>({new Token("break", TokenType::Break),
                                     new Token(";", TokenType::S_Colon)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::ChildStatement);
  EXPECT_EQ(ast->children[0]->token.type, TokenType::Break);
}

TEST(shouldParseBreakInFor, forParser) {
  auto tokens = new vector<Token *>(
      {new Token("for", TokenType::For),     new Token("(", TokenType::L_PH),
       new Token("i", TokenType::Var),       new Token("=", TokenType::Assign),
       new Token("0", TokenType::Num),       new Token(";", TokenType::S_Colon),
       new Token("i", TokenType::Var),       new Token("<", TokenType::Lt),
       new Token("10", TokenType::Num),      new Token(";", TokenType::S_Colon),
       new Token("i", TokenType::Var),       new Token("++", TokenType::Inc),
       new Token(")", TokenType::R_PH),      new Token("{", TokenType::L_BR),
       new Token("k", TokenType::Var),       new Token("=", TokenType::Assign),
       new Token("1", TokenType::Num),       new Token(";", TokenType::S_Colon),
       new Token("break", TokenType::Break), new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->children[3]->children[1]->children[0]->token.type,
            TokenType::Break);
}

TEST(shouldParseBreakInWhile, whileParser) {
  auto tokens = new vector<Token *>(
      {new Token("for", TokenType::For),     new Token("(", TokenType::L_PH),
       new Token("i", TokenType::Var),       new Token("=", TokenType::Assign),
       new Token("0", TokenType::Num),       new Token(";", TokenType::S_Colon),
       new Token("i", TokenType::Var),       new Token("<", TokenType::Lt),
       new Token("10", TokenType::Num),      new Token(";", TokenType::S_Colon),
       new Token("i", TokenType::Var),       new Token("++", TokenType::Inc),
       new Token(")", TokenType::R_PH),      new Token("{", TokenType::L_BR),
       new Token("k", TokenType::Var),       new Token("=", TokenType::Assign),
       new Token("1", TokenType::Num),       new Token(";", TokenType::S_Colon),
       new Token("break", TokenType::Break), new Token(";", TokenType::S_Colon),
       new Token("}", TokenType::R_BR)});
  auto ast = Parser::parseTokens(*tokens, 0, (int)tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->children[3]->children[1]->children[0]->token.type,
            TokenType::Break);
}

// TODO: add more test if we have multiple ; in one line

TEST(shouldParseMultiSColonInBlock, blockParser) {
  auto tokens = new vector<Token *>(
      {new Token("{", TokenType::L_BR, 1), new Token("if", TokenType::If, 2),
       new Token("(", TokenType::L_PH, 2), new Token("a", TokenType::Var, 2),
       new Token(">", TokenType::Gt, 2), new Token("0", TokenType::Num, 2),
       new Token(")", TokenType::R_PH, 2),
       new Token(";", TokenType::S_Colon, 3),
       new Token(";", TokenType::S_Colon, 3),
       new Token(";", TokenType::S_Colon, 3),
       new Token("}", TokenType::R_BR, 4)});
  ParserFun blockParser = Parser::getUnFinalParser(TokenType::L_BR);
  auto ast = blockParser(*tokens, 0, (int)tokens->size(), 0);
  EXPECT_EQ(ast->type, ASTType::Block);
  EXPECT_EQ(ast->children[0]->children[1]->children.size(), 0);
}

TEST(shouldParseOneSColonInBlock, blockParser) {
  auto tokens = new vector<Token *>(
      {new Token("{", TokenType::L_BR, 1), new Token("if", TokenType::If, 2),
       new Token("(", TokenType::L_PH, 2), new Token("a", TokenType::Var, 2),
       new Token(">", TokenType::Gt, 2), new Token("0", TokenType::Num, 2),
       new Token(")", TokenType::R_PH, 2),
       new Token(";", TokenType::S_Colon, 3),
       new Token("}", TokenType::R_BR, 4)});
  ParserFun blockParser = Parser::getUnFinalParser(TokenType::L_BR);
  auto ast = blockParser(*tokens, 0, (int)tokens->size(), 0);
  EXPECT_EQ(ast->type, ASTType::Block);
  EXPECT_EQ(ast->children[0]->children[1]->children.size(), 0);
}
