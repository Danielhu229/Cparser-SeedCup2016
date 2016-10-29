//
// Created by aLIEzTed on 10/29/16.
//

#include <gtest/gtest.h>
#include "Expr.h"
TEST(shouldGetIfExpr, parseIfExpr) {
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
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldGetIfExprNoBrackets, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldGetIfElseExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::S_Colon);
}

TEST(shouldGetIfNoBracketsElseExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldGetIfElseNoBracketsExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("1", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("else", TokenType::Else), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon)
                                    });

  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[1]->children[1]->children[0]->token->token, TokenType::Assign);
}

TEST(shouldGetIfElseEitherNoBracketsExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon)
                                    });

  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::Dec);
}

TEST(shouldGetIfElseIfExpr, parseIfExpr) {
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
                                        new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("h", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->children[0]->token->token,
            TokenType::Var);
}

TEST(shouldGetIfElseIfNoneBracketsExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("else", TokenType::Else), new Token("h", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::If);
}

TEST(shouldGetIfNoBracketsElseIfExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("else", TokenType::Else),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("==", TokenType::Eq),
                                        new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("h", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->children[0]->token->token,
            TokenType::Var);
}

TEST(shouldGetIfElseIfNoBracketsExpr, parseIfExpr) {
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
                                        new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("h", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token->token, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token->token,
            TokenType::Var);

}

TEST(shouldGetMultiIfElseIfExpr, parseIfExpr) {
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
                                        new Token("0", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("j", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("<", TokenType::Lt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR), new Token("h", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("k", TokenType::Var),
                                        new Token("++", TokenType::Inc), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token->token, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token->token, TokenType::S_Colon);
}

TEST(shouldGetVarDec, parseDeclare) {
  auto tokens = new vector<Token *>({
                                        new Token("int", TokenType::Int), new Token("a", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("1", TokenType::Num),
                                        new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Assign);
}

TEST(shouldGetWhileExpr, parseWhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldGetWhileNoBrackets, parseWhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token->token, TokenType::Inc);
}

TEST(shouldGetForExpr, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::For);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Assign);
  EXPECT_EQ(expr.statements[0]->children[1]->token->token, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[2]->token->token, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[3]->token->token, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->children[0]->token->token, TokenType::Inc);

}

TEST(shouldGetExpr, parseExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
//                                        new Token("j", TokenType::Var), new Token("=", TokenType::Assign),
//                                        new Token("5", TokenType::Num), new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::S_Colon);
}

TEST(shouldGetMultiExpr, parseExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("j", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("5", TokenType::Num), new Token(";", TokenType::S_Colon)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[1]->token->token, TokenType::S_Colon);
}

TEST(shouldGetMultiExprComplex, parse) {
  auto tokens = new vector<Token *>({
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("j", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("5", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 3);
  EXPECT_EQ(expr.statements[0]->children[0]->token->token, TokenType::Assign);
  EXPECT_EQ(expr.statements[1]->children[0]->token->token, TokenType::Assign);
  EXPECT_EQ(expr.statements[2]->token->token, TokenType::For);

}

TEST(shouldGetForIfExprComplex, parse) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR), new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token->token, TokenType::For);
  EXPECT_EQ(expr.statements[1]->token->token, TokenType::If);
}

TEST(shouldGetForWhileExprComplex, parse) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)
                                    });
  Expr expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
}