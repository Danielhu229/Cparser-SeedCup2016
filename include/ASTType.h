//
// Created by Danielhu <yimingdz@gmail.com> on 16/10/25.
//

#ifndef C_PARSER_ASTTYPE_H
#define C_PARSER_ASTTYPE_H
namespace cParser {

enum class ASTType {
  Final,
  Binary,
  Call,
  If,
  Else,
  ElseIf,
  For,
  LSelf,
  RSelf,
  DeclareVar,
  ChildStatement,
  Comma,
  While,
  Switch,
  Do,
  Block,
  Printf
};
} // namespace cParser
#endif // C_PARSER_ASTTYPE_H
