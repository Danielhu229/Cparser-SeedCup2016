//
// Created by aLIEzTed on 10/29/16.
//

#include <gtest/gtest.h>
#include "AheadWatcher.h"

using namespace cParser;

TEST(shouldGetIfExpr, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Inc),
                                        new Token("4", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token.type, TokenType::Inc);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::Inc);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::S_Colon);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->children[0]->token.type, TokenType::Dec);
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

  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[1]->children[1]->children[0]->token.type, TokenType::Assign);
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

  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Dec);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token.type, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->children[0]->token.type,
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::If);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token.type, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->children[0]->token.type,
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->children[0]->token.type, TokenType::Dec);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[2]->children[0]->children[0]->token.type,
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Eq);
  EXPECT_EQ(expr.statements[0]->children[2]->children[1]->children[0]->token.type, TokenType::S_Colon);
}

TEST(shouldGetVarDec, parseDeclare) {
  auto tokens = new vector<Token *>({
                                        new Token("int", TokenType::Int), new Token("a", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("1", TokenType::Num),
                                        new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::S_Colon);
  // EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Int);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token.type, TokenType::Inc);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::Inc);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::For);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[0]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::Lt);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[2]->children[0]->token.type, TokenType::Inc);
  EXPECT_EQ(expr.statements[0]->children[3]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->children[0]->token.type, TokenType::Inc);

}

TEST(shouldGetExpr, parseExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
//                                        new Token("j", TokenType::Var), new Token("=", TokenType::Assign),
//                                        new Token("5", TokenType::Num), new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::S_Colon);
}

TEST(shouldGetMultiExpr, parseExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("j", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("5", TokenType::Num), new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[1]->token.type, TokenType::S_Colon);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 3);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(expr.statements[1]->children[0]->token.type, TokenType::Assign);
  EXPECT_EQ(expr.statements[2]->token.type, TokenType::For);

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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::For);
  EXPECT_EQ(expr.statements[1]->token.type, TokenType::If);
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
}

TEST(shouldGetIfForExprComplex, parse) {
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
                                        new Token("}", TokenType::R_BR),

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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[1]->token.type, TokenType::For);
}

TEST(shouldGetIfWhileExprComplex, parse) {
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
                                        new Token("}", TokenType::R_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR)

                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[1]->token.type, TokenType::While);
}

TEST(shouldGetWhileInsideIf, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),

                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[2], nullptr);
}

TEST(shouldGetWhileNoBracketsInsideIf, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[2], nullptr);
}

TEST(shouldGetWhileInsideIfNoneBrackets, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
}

TEST(shouldGetWhileInsideIfElse, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("else", TokenType::Else), new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::L_BR);
}

TEST(shouldGetWhileInsideIfElseNoBrackets, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),

                                        new Token("}", TokenType::R_BR),

                                        new Token("else", TokenType::Else),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::While);
}

TEST(shouldGetWhileInsideIfElseNoneBrackets, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),

                                        new Token("else", TokenType::Else),
                                        new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::While);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::S_Colon);
}

TEST(shouldGetForInsideIf, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

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

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::For);
  EXPECT_EQ(expr.statements[0]->children[2], nullptr);
}

TEST(shouldGetForNoBracketsInsideIf, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::For);
  EXPECT_EQ(expr.statements[0]->children[2], nullptr);
}

TEST(shouldGetForInsideIfNoneBrackets, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If, 1), new Token("(", TokenType::L_PH, 1),
                                        new Token("a", TokenType::Var, 1), new Token(">", TokenType::Gt, 1),
                                        new Token("0", TokenType::Num, 1),
                                        new Token(")", TokenType::R_PH, 1),

                                        new Token("for", TokenType::For, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("i", TokenType::Var, 2),
                                        new Token("=", TokenType::Assign, 2), new Token("0", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("<", TokenType::Lt, 2), new Token("5", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("++", TokenType::Inc, 2),
                                        new Token(")", TokenType::R_PH, 2),
                                        new Token("a", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::For);
  EXPECT_EQ(expr.statements[0]->children[2], nullptr);
}

TEST(shouldGetForInsideIfElse, parseIfExpr) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),
                                        new Token("{", TokenType::L_BR),
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
                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::L_BR);
}

TEST(shouldGetForNoBracketsInsideIfElse, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var),
                                        new Token("--", TokenType::Dec), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR), new Token("else", TokenType::Else),

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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[2]->token.type, TokenType::For);
}

TEST(shouldGetDoWhileExpr, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::DO);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[0]->children[0]->token.type, TokenType::Ne);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->children[0]->token.type, TokenType::Inc);
}

TEST(shouldGetDoWhileNoBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO, 1),
                                        new Token("a", TokenType::Var, 2), new Token("++", TokenType::Inc, 2),
                                        new Token(";", TokenType::S_Colon, 2),
                                        new Token("while", TokenType::While, 3), new Token("(", TokenType::L_PH, 3),
                                        new Token("a", TokenType::Var, 3), new Token("!=", TokenType::Ne, 3),
                                        new Token("10", TokenType::Num, 3), new Token(")", TokenType::R_PH, 3),
                                        new Token(";", TokenType::S_Colon, 3)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::S_Colon);
}

TEST(shouldGetForInDoWhileNoBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO, 1),

                                        new Token("for", TokenType::For, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("i", TokenType::Var, 2),
                                        new Token("=", TokenType::Assign, 2), new Token("0", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("<", TokenType::Lt, 2), new Token("5", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("++", TokenType::Inc, 2),
                                        new Token(")", TokenType::R_PH, 2), new Token("{", TokenType::L_BR, 2),
                                        new Token("a", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3), new Token("}", TokenType::R_BR, 4),

                                        new Token("while", TokenType::While, 5), new Token("(", TokenType::L_PH, 5),
                                        new Token("a", TokenType::Var, 5), new Token("!=", TokenType::Ne, 5),
                                        new Token("10", TokenType::Num, 5), new Token(")", TokenType::R_PH, 5),
                                        new Token(";", TokenType::S_Colon, 5)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetIfInDoWhileNoBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO, 1),

                                        new Token("if", TokenType::If, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("a", TokenType::Var, 2), new Token(">", TokenType::Gt, 2),
                                        new Token("0", TokenType::Num, 2),
                                        new Token(")", TokenType::R_PH, 2),
                                        new Token("{", TokenType::L_BR, 2),
                                        new Token("i", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3),
                                        new Token("}", TokenType::R_BR, 4),

                                        new Token("while", TokenType::While, 5), new Token("(", TokenType::L_PH, 5),
                                        new Token("a", TokenType::Var, 5), new Token("!=", TokenType::Ne, 5),
                                        new Token("10", TokenType::Num, 5), new Token(")", TokenType::R_PH, 5),
                                        new Token(";", TokenType::S_Colon, 5)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetIfNoBracketsInDoWhileNoBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO, 1),

                                        new Token("if", TokenType::If, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("a", TokenType::Var, 2), new Token(">", TokenType::Gt, 2),
                                        new Token("0", TokenType::Num, 2),
                                        new Token(")", TokenType::R_PH, 2),
                                        new Token("i", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3),

                                        new Token("while", TokenType::While, 4), new Token("(", TokenType::L_PH, 4),
                                        new Token("a", TokenType::Var, 4), new Token("!=", TokenType::Ne, 4),
                                        new Token("10", TokenType::Num, 4), new Token(")", TokenType::R_PH, 4),
                                        new Token(";", TokenType::S_Colon, 4)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetForNoBracketsInDowhileNoBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO, 1),

                                        new Token("for", TokenType::For, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("i", TokenType::Var, 2),
                                        new Token("=", TokenType::Assign, 2), new Token("0", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("<", TokenType::Lt, 2), new Token("5", TokenType::Num, 2),
                                        new Token(";", TokenType::S_Colon, 2), new Token("i", TokenType::Var, 2),
                                        new Token("++", TokenType::Inc, 2),
                                        new Token(")", TokenType::R_PH, 2),
                                        new Token("a", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3),

                                        new Token("while", TokenType::While, 4), new Token("(", TokenType::L_PH, 4),
                                        new Token("a", TokenType::Var, 4), new Token("!=", TokenType::Ne, 4),
                                        new Token("10", TokenType::Num, 4), new Token(")", TokenType::R_PH, 4),
                                        new Token(";", TokenType::S_Colon, 4)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetComplexExpr1, parse) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),

                                        new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("b", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("1", TokenType::Num), new Token("+", TokenType::Add),
                                        new Token("2", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),

                                        new Token("else", TokenType::Else), new Token("{", TokenType::L_BR),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token(">=", TokenType::Ge),
                                        new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("--", TokenType::Dec),
                                        new Token(")", TokenType::R_PH),

                                        new Token("{", TokenType::L_BR), new Token("k", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("10", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("k", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("10", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);

}

TEST(shouldGetComplexExpr2, parse) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("b", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("1", TokenType::Num), new Token("+", TokenType::Add),
                                        new Token("2", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),

                                        new Token("{", TokenType::L_BR),

                                        new Token("}", TokenType::R_BR),
                                        new Token("else", TokenType::Else),

                                        new Token("{", TokenType::L_BR),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("=", TokenType::Assign),
                                        new Token("10", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token(">=", TokenType::Ge),
                                        new Token("0", TokenType::Num), new Token(";", TokenType::S_Colon),
                                        new Token("a", TokenType::Var), new Token("--", TokenType::Dec),
                                        new Token(")", TokenType::R_PH),

                                        new Token("{", TokenType::L_BR), new Token("k", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("10", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("k", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("10", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),
                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);

}

TEST(shouldGetComplexExprNoneBrackets, parse) {
  auto tokens = new vector<Token *>({

                                        new Token("if", TokenType::If, 1), new Token("(", TokenType::L_PH, 1),
                                        new Token("a", TokenType::Var, 1), new Token(">", TokenType::Gt, 1),
                                        new Token("0", TokenType::Num, 1),
                                        new Token(")", TokenType::R_PH, 1),

                                        new Token("while", TokenType::While, 2), new Token("(", TokenType::L_PH, 2),
                                        new Token("i", TokenType::Var, 2),
                                        new Token("<", TokenType::Lt, 2), new Token("10", TokenType::Num, 2),
                                        new Token(")", TokenType::R_PH, 2),

                                        new Token("a", TokenType::Var, 3), new Token("++", TokenType::Inc, 3),
                                        new Token(";", TokenType::S_Colon, 3),

                                        new Token("else", TokenType::Else, 4),

                                        new Token("for", TokenType::For, 5), new Token("(", TokenType::L_PH, 5),
                                        new Token("a", TokenType::Var, 5), new Token("=", TokenType::Assign, 5),
                                        new Token("10", TokenType::Num, 5), new Token(";", TokenType::S_Colon, 5),
                                        new Token("a", TokenType::Var, 5), new Token(">=", TokenType::Ge, 5),
                                        new Token("0", TokenType::Num, 5), new Token(";", TokenType::S_Colon, 5),
                                        new Token("a", TokenType::Var, 5), new Token("--", TokenType::Dec, 5),
                                        new Token(")", TokenType::R_PH, 5),

                                        new Token("k", TokenType::Var, 6),
                                        new Token("=", TokenType::Assign, 6), new Token("10", TokenType::Num, 6),
                                        new Token(";", TokenType::S_Colon, 6)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetMultiSCoonIfExpr, parse) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If, 1), new Token("(", TokenType::L_PH, 1),
                                        new Token("a", TokenType::Var, 1), new Token(">", TokenType::Gt, 1),
                                        new Token("0", TokenType::Num, 1),
                                        new Token(")", TokenType::R_PH, 1),
                                        new Token("{", TokenType::L_BR, 1),
                                        new Token(";", TokenType::S_Colon, 2),
                                        new Token(";", TokenType::S_Colon, 2),
                                        new Token("}", TokenType::R_BR, 3),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
  EXPECT_EQ(expr.statements[0]->children[0]->token.type, TokenType::Gt);
  EXPECT_EQ(expr.statements[0]->children[1]->token.type, TokenType::L_BR);
  EXPECT_EQ(expr.statements[0]->children[1]->children[0]->token.type, TokenType::S_Colon);
}

TEST(shouldGetDoubleIf, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });

  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
}

TEST(shouldGetDoubleFor, parseForExpr) {
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
}

TEST(shouldGetDoubleWhile, parseWhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
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
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
}

TEST(shouldGetUltraIf, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("i", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("j", TokenType::Var), new Token("--", TokenType::Dec),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("k", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 3);
}

TEST(shouldGetIfInsideFor, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Inc),
                                        new Token("4", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->children[1]->token.type, TokenType::L_BR);
}

TEST(shouldGetWhileInsideFor, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token(";", TokenType::S_Colon),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("4", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements[0]->children[3]->children[0]->children.size(), 1);
  EXPECT_EQ(expr.statements[0]->children[3]->children[1]->children.size(), 2);
}

TEST(shouldGetDowhileInsideIf, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetDowhileInsideFor, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetDowhileInsideWhile, parseWhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
}

TEST(shouldGetDoubleDowhile, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 2);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::DO);
  EXPECT_EQ(expr.statements[1]->token.type, TokenType::DO);
}


TEST(shouldGetForInsideFor, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetForNobrackets, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For, 1), new Token("(", TokenType::L_PH, 1),
                                        new Token("i", TokenType::Var, 1),
                                        new Token("=", TokenType::Assign, 1), new Token("0", TokenType::Num, 1),
                                        new Token(";", TokenType::S_Colon, 1), new Token("i", TokenType::Var, 1),
                                        new Token("<", TokenType::Lt, 1), new Token("5", TokenType::Num, 1),
                                        new Token(";", TokenType::S_Colon, 1), new Token("i", TokenType::Var, 1),
                                        new Token("++", TokenType::Inc, 1),
                                        new Token(")", TokenType::R_PH, 1),
                                        new Token("a", TokenType::Var, 2), new Token("++", TokenType::Inc, 2),
                                        new Token(";", TokenType::S_Colon, 2),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
}


TEST(shouldGetForInsideNonebracketsFor, parseForExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH),

                                        new Token("for", TokenType::For), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("=", TokenType::Assign), new Token("0", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("5", TokenType::Num),
                                        new Token(";", TokenType::S_Colon), new Token("i", TokenType::Var),
                                        new Token("++", TokenType::Inc),
                                        new Token(")", TokenType::R_PH),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetWhileInsideWhile, parseWhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),

                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("i", TokenType::Var),
                                        new Token("<", TokenType::Lt), new Token("10", TokenType::Num),
                                        new Token(")", TokenType::R_PH), new Token("{", TokenType::L_BR),
                                        new Token("a", TokenType::Var), new Token("++", TokenType::Inc),
                                        new Token(";", TokenType::S_Colon), new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
}

TEST(shouldGetDowhileInsideDowhile, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("do", TokenType::DO), new Token("{", TokenType::L_BR),
                                        new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
}

TEST(shouldGetDowhileInsideDowhileNoneBrackets, parseDowhileExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("do", TokenType::DO),
                                        new Token("do", TokenType::DO),
                                        new Token("do", TokenType::DO),
                                        new Token("i", TokenType::Var), new Token(";", TokenType::S_Colon),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("while", TokenType::While), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token("!=", TokenType::Ne),
                                        new Token("10", TokenType::Num), new Token(")", TokenType::R_PH),
                                        new Token(";", TokenType::S_Colon)
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
}

TEST(shouldGetIfInsideIf, parseIfExpr) {
  auto tokens = new vector<Token *>({
                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),

                                        new Token("if", TokenType::If), new Token("(", TokenType::L_PH),
                                        new Token("a", TokenType::Var), new Token(">", TokenType::Gt),
                                        new Token("0", TokenType::Num),
                                        new Token(")", TokenType::R_PH),
                                        new Token("{", TokenType::L_BR),
                                        new Token("i", TokenType::Var), new Token("=", TokenType::Inc),
                                        new Token("4", TokenType::Num),
                                        new Token(";", TokenType::S_Colon),
                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),

                                        new Token("}", TokenType::R_BR),
                                    });
  AheadWatcher expr(*tokens);
  bool rst = expr.parse();
  EXPECT_EQ(true, rst);
  EXPECT_EQ(expr.statements.size(), 1);
  EXPECT_EQ(expr.statements[0]->token.type, TokenType::If);
}