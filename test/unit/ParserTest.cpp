//
// Created by 胡一鸣 on 16/10/26.
//

#include "Parser.h"
#include "Token.h"
#include "TokenType.h"
#include "gtest/gtest.h"

using namespace cParser;

TEST(shouldDivideTokens, binaryParser) {
  auto tokens = new vector<Token *>({new Token("3435", TokenType::Num),
                                     new Token("+", TokenType::Add),
                                     new Token("78", TokenType::Num)});
  ParserFun binaryParser = Parser::getUnFinalParser(TokenType::Add);
  auto ast = binaryParser(*tokens, 0, 3, 1);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[0]->token->name, "3435");
  EXPECT_EQ(ast->children[1]->token->name, "78");
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
  EXPECT_EQ(ast->token->name, "*");
  EXPECT_EQ(ast->children[0]->token->name, "+");
  EXPECT_EQ(ast->children[1]->token->name, "57");
}

TEST(shouldParseLeft, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("++", TokenType::Inc), new Token("i", TokenType::Var)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, tokens->size(), 0);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token->name, "i");
}

TEST(shouldParseRight, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("i", TokenType::Var), new Token("--", TokenType::Dec)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, tokens->size(), 1);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token->name, "i");
}

TEST(shouldParseComplex, SingleParser) {
  auto tokens = new vector<Token *>(
      {new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
       new Token("+", TokenType::Add), new Token("a", TokenType::Var),
       new Token("--", TokenType::Dec)});
  ParserFun selfPerser = Parser::getUnFinalParser(TokenType::Inc);
  auto ast = selfPerser(*tokens, 0, 2, 1);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token->name, "i");

  ast = selfPerser(*tokens, 3, 5, 4);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token->name, "a");
}

TEST(shouldParseExpression, Parse) {
  auto tokens = new vector<Token *>({
      new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
      new Token("+", TokenType::Add), new Token("a", TokenType::Var),
      new Token("--", TokenType::Dec), new Token("*", TokenType::Mul),
      new Token("4", TokenType::Num)
  });
  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  EXPECT_EQ(ast->type, ASTType::Binary);
  EXPECT_EQ(ast->token->token, TokenType::Add);
}

TEST(shouldParseIfExpression, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token("}", TokenType::R_BR),
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
}

TEST(shouldParseIfExpressionCond, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldParseIfElseExpression, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->children[0]->token->token, TokenType::Dec);
}


TEST(shouldParseIfNoBracketsElse, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldParseIfElseNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldParserIfElseIf, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(ast->children[2]->children[2], nullptr);
}

TEST(shouldParserIfElseIfNoneBrackets, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else), new Token("if", TokenType::If),
                                        new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldParserIfElseIfNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("else", TokenType::Else), new Token("if", TokenType::If),
                                        new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token->token, TokenType::Dec);
}


TEST(shouldParseIfNoBracketsElseIf, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else), new Token("if", TokenType::If),
                                        new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::If);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(ast->children[2]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[1]->children[0]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldParseIfElseEitherNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldParseIfExpressionComplex, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children.size(), 3);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(ast->children[2], nullptr);
}

TEST(shouldParseIfNoBrackets, ifParser) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::If);
  EXPECT_EQ(ast->token->token, TokenType::If);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[2], nullptr);
}


TEST(shouldParseWhileExpr, whileParser) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token->token, TokenType::While);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldParseWhileExprInBlock, whileParser) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token->token, TokenType::While);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children.size(), 2);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[1]->children[1]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(ast->children[1]->children[1]->token->token, TokenType::S_Colon);
}

TEST(shouldParseWhileNoBrackets, whileParser) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::While);
  EXPECT_EQ(ast->token->token, TokenType::While);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldParseForExpr, forParser) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token->token, TokenType::For);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[3]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->children[0]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldParseForExprInBlock, forParser) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("a", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("1", TokenType::Num),
                                        new Token("+", TokenType::Add), new Token("2", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token->token, TokenType::For);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->token->token, TokenType::L_BR);
  EXPECT_EQ(ast->children[0]->children[0]->token->token, TokenType::Assign);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Lt);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->children[1]->children[0]->token->token, TokenType::Assign);
}

TEST(shouldParseForExprNoBrackets, forParser) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, (int) tokens->size());
  EXPECT_EQ(ast->type, ASTType::For);
  EXPECT_EQ(ast->token->token, TokenType::For);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[3]->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children[0]->children[0]->token->token, TokenType::Assign);
  EXPECT_EQ(ast->children[1]->children[0]->token->token, TokenType::Lt);
  EXPECT_EQ(ast->children[2]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(ast->children[3]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldParseSimiColon, Parse) {
  auto tokens = new vector<Token *>({
                                        new Token("4", TokenType::Num),
                                        new Token(";", TokenType::S_Colon)
                                    });
  auto ast = Parser::parseTokens(*tokens, 0, tokens->size());
  EXPECT_EQ(ast->type, ASTType::ChildStatement);
  EXPECT_EQ(ast->token->token, TokenType::S_Colon);
  EXPECT_EQ(ast->children.size(), 1);
  EXPECT_EQ(ast->children[0]->token->token, TokenType::Num);
}
