#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include "pch.h"

// enum LEXEME_TYPE aka LexemeType & LexType
// defines specifics and roles of lexemes present in content
//
// -------------+---------------------------------------------------------------+
// LEXEME_IDN   | Representation of an identifier. variable names, lables etc...|
// LEXEME_PUN   | Representation of punctuations. symbols such as '%',':' etc...|
// LEXEME_KEY   | Representation of pre defined keywords such as add, sub etc...|
// LEXEME_LIT   | Representation of special data such as strings, numeric etc...|
// LEXEME_OPR   | Representation of operations such as negetive, pointers etc...|
// LEXEME_NUL   | Representation of insignificant cor ignorable charachters     |
// -------------+---------------------------------------------------------------+
typedef enum LEXEME_TYPE {
    LEXEME_IDN,     // Identifiers
    LEXEME_PUN,     // Punctuations
    LEXEME_KEY,     // Keywords
    LEXEME_LIT,     // Literals
    LEXEME_OPR,     // Operations
    LEXEME_NUL      // Null
} LexemeType, LexType;

#endif
