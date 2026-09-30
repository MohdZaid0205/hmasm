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
// LEXEME_NUL   | Representation of unrecognized charachters                    |
// -------------+---------------------------------------------------------------+
typedef enum LEXEME_TYPE {
    LEXEME_IDN,     // Identifiers
    LEXEME_PUN,     // Punctuations
    LEXEME_KEY,     // Keywords
    LEXEME_LIT,     // Literals
    LEXEME_OPR,     // Operations
    LEXEME_NUL,     // Null
} LexemeType, LexType;

typedef struct LEXEME_IDENTIFIER {
    unsigned int l_no;
    unsigned int c_no;
    unsigned int size;
    char* data;
} LexemeIdentifier, LexIdn;

void _print_lexeme_idn(struct LEXEME_IDENTIFIER* idn);

typedef struct LEXEME_PUNCTUATION {
    unsigned int l_no;
    unsigned int c_no;
    char  data;
} LexemePunctuation, LexPun;

void _print_lexeme_pun(struct LEXEME_PUNCTUATION* pun);

typedef struct LEXEME_KEYWORD {
    unsigned int l_no;
    unsigned int c_no;
    unsigned int size;
    char* data;
} LexemeKeyword, LexKey;

void _print_lexeme_key(struct LEXEME_KEYWORD* key);

typedef enum LEXEME_LITERAL_TYPE {
    LITERAL_NUMERIC,
    LITERAL_COMMENT,
    LITERAL_STRING
} LexemeLiteralType, LexLitType;

typedef struct LEXEME_LITERAL {
    enum LEXEME_LITERAL_TYPE type;
    unsigned int l_no;
    unsigned int c_no;
    unsigned int size;
    char* data;
} LexemeLiteral, LexLit;

void _print_lexeme_lit(struct LEXEME_LITERAL* lit);

typedef struct LEXEME_OPERATION {
    unsigned int l_no;
    unsigned int c_no;
    char  data;
} LexemeOperation, LexOpr;

void _print_lexeme_opr(struct LEXEME_OPERATION* opr);

typedef struct LEXEME_NULL {
    unsigned int l_no;
    unsigned int c_no;
    char  data;
} LexemeNull, LexNul;

void _print_lexeme_nul(struct LEXEME_NULL* nul);

typedef struct LEXEME {
    enum LEXEME_TYPE type;
    union {
        struct LEXEME_IDENTIFIER    idn;
        struct LEXEME_PUNCTUATION   pun;
        struct LEXEME_KEYWORD       key;
        struct LEXEME_LITERAL       lit;
        struct LEXEME_OPERATION     opr;
        struct LEXEME_NULL          nul;
    } as;
} Lexeme, Lex;

void _print_lexeme(struct LEXEME* lex);

#endif
