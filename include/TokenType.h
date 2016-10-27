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
  /*!
   * keyword char
   */
  Char,
  Else,
  Enum,
  If,
  ElseIf,
  Int,
  Double,
  Float,
  Long,
  Unsigned,
  /*!
   * keyword return
   */
  Return,
  /*!
   * keyword sizeof
   */
  Sizeof,
  /*!
   * keyword while
   */
  While,
  /*!
   * keyword do
   */
  DO,
  /*!
   * symbol for =
   */
  Assign, // =
  /*!
   * symbol for ?
   */
  Cond, // ?
  /*!
   * symbol for ||
   */
  L_or, // ||
  /*!
   * symbol for &&
   */
  L_an, // &&
  /*!
   * symbol for ==
   */
  Eq, // ==
  /*!
   * symbol for !=
   */
  Ne, // !=
  /*!
   * symbol for less than <
   */
  Lt,
  /*!
   * symbol for >
   */
  Gt, // < and >
  /*!
   * symbol for <=
   */
  Le,
  /*!
   * symbol for >=
   */
  Ge,
  Add, Sub, Mul, Div, Mod,
  /*!
   * symbol for increment ++
   */
  Inc,
  /*!
   * symbol for decrement --
   */
  Dec,
  /*!
   * keyword break;
   */
  Break,
  /*!
   * keyword switch
   */
  Switch,
  /*!
   * keyword case
   */
  Case,
  /*!
   * for
   */
  For,
  /*!
   * keyword void
   */
  Void,
  /*!
   * keyword main
   */
  Main, // main
  /*!
   * punctuation [
   */
  L_BRAK,
  /*!
   * punctuation ]
   */
  R_BRAK,
  /*!
   * punctuation (
   */
  L_PH,
  /*!
   * punctuation )
   */
  R_PH,
  /*!
   * punctuation {
   */
  L_BR,
  /*!
   * punctuation }
   */
  R_BR,
  /*!
   * punctuation :
   */
  Colon,
  /*!
   * punctuation ;
   */
  S_Colon,
  /*!
   * punctuation ,
   */
  Comma,
  /*!
   * punctuation !
   */
  Not
};


#endif //SEEDCUP_SYMBOL_H
