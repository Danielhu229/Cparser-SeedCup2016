//
// Created by aLIEzTed on 10/24/16.
//

#ifndef SEEDCUP_SYMBOL_H
#define SEEDCUP_SYMBOL_H
// tokens and classes
enum class TokenType {

  Num, // numbers and single character
  Fun, Sys, Glo, Loc,
  Var, // Variable
  Str, // String literal
  Char, Else, Enum, If, Int, Double, Float, Long, Unsigned, Return, Sizeof, While, DO,
  Assign, // =
  Cond, // ?
  L_or, // ||
  L_an, // &&
  Eq, // ==
  Ne, // !=
  Lt, Gt, // < and >
  Le, Ge, // <= and >=
  Add, Sub, Mul, Div, Mod, Inc, Dec,
  Break, // Break;
  Switch,
  Case,
  For,
  Void,
  Main, // main
  L_BRAK, R_BRAK, // [ and ]
  L_PH, R_PH, // { and }
  L_BR, R_BR, // ( and )
  Colon, S_Colon, Comma, //: and ; and ,
  Not // !
};


#endif //SEEDCUP_SYMBOL_H
