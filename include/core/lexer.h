#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include "pch.h"

// [ INFO ] --------------------------------------------------------------------+
// lexer for assembler to directly operate on a FILE* while seperating const	| 
// char* into their own small lexemes, essentially there are 4 types of lexemes	|
// as prev implementeation of LEXER_H (classifications) CFL_XXX					|
//																				|
// * PUNCTUATION: single charachter that has associated meaning with it.		|
// * OPERATIONS : single or multicharachter symbols with associated opration.	|
// * LITERAL	: literal types such as string, int or identifiers.				|
// * KEYWORDS	: set of words reserved by assembler to identify operations.	|
// -----------------------------------------------------------------------------+

// enum ASSEMBLER_LEXEME_TYPE containing possible types of lexeme that can be 
// constructed using ASSEMBLER_LEXER_H lexer aimed to seperate charachters.
typedef enum ASSEMBLER_LEXEME_TYPE {
	LEXEME_PUN,							// LEXEME type found to be PUNCTUATION
	LEXEME_OPR,							// LEXEME type found to be OPERATION
	LEXEME_LIT,							// LEXEME type found to be LITERAL
	LEXEME_KEY,							// LEXEME type found to be KEYWORD
} LexemeType;

// [ IDEA ] --------------------------------------------------------------------+
// So we first define all structures that store corresponding lexeme and then	|
// use those structures and directly cast them to required types that we		|
// require such as:																|
//		LEXEME_TOKEN.type	: is typeof(ASSEMBLER_LEXEME_TYPE)					|
//		LEXEME_TOKEN.as.pun : is typeof(<pun-type>)								|
//		LEXEME_TOKEN.as.opr : is typeof(<opr-type>)								|
//		LEXEME_TOKEN.as.lit : is typeof(<lit-type>)								|
//                                                                              |
// [ CHANGE_LOG ] --------------------------------------------------------------+
// ---------------------------(25-12-2025 17:03)--------------------------------+
// -> removed line:                                                             |
//		LEXEME_TOKEN.as.key : is typeof(<key-type>)								|
//		I dont think lexer needs idea of keywords, moving this to parser that   |
//      looks at LITERAL_NONE and checks against list of keywords               |
// -----------------------------------------------------------------------------+
// *(this idea is subjected to change)											|
// -----------------------------------------------------------------------------+

// struct LEXEME_PUNCTUATION;              // forward declaration of Structure
// struct LEXEME_OPERATION;                // forward declaration of Structure
// struct LEXEME_LITERAL;                  // forward declaration of Structure

// LEXEME_PUNCTUATION aka LexemePun & LexemePunctuation
// LEXEME_PUNCTUATION is representation of any punctuataion that is allowed inside
// assembly grammar, see GRAMMAR.md for TERMINAL values and symbols, following is
// a list of example punctuations that are found in example assembly files
//
// -----------------------------------------------------------------------------+
// DOT   (.) -> sub-labels like `.label` or with predefined types `.data`       |
// COMMA (,) -> seperators between arguments to instructions, `li a0, 0x01`     |
// ...                                                                          |
// -----------------------------------------------------------------------------+
typedef struct LEXEME_PUNCTUATION{
    unsigned int line_no;               // line number within file OCCURANCE
    unsigned int char_no;               // charcahter number within that LINE
    char data;                          // data to store (associated char).
} LexemePun, LexemePunctuation;

// LEXEME_OPERATION aka LexemeOpr & LexemeOperation
// LEXEME_OPERATION is representation of operations that is allowed inside assem-
// -bly, what do i call operation? anything arithmatic that has directly been put
// in place inside assembly file, however this is to be used only when dealing
// with addresses and constant expressions in general sense, following is a list
// of example operations according to this project
//
// --------------+--------------------------------------------------------------+
// ADD       (+) | for example we jump relative to label by `LABEL+0x08`        |
// SUBTRACT  (-) |             we mutiply value with a value 0x08*0x0ff         |
// MULTIPLY  (*) | it is however important to note that this is only to be used |
// DIVIDE    (/) | at compile time, not at runtime [1]                          |
// --------------+--------------------------------------------------------------+
//
// [1]: using LABEL+value or value+value is not same as doing assembly operation
//      as add LABEL, value or add value, value, rather it is to be considerd as 
//      macro for making calculations easier
typedef struct LEXEME_OPERATION{
    unsigned int line_no;               // line number within file OCCURANCE
    unsigned int char_no;               // charcahter number within that LINE
    char data;                          // data to store (associated symbol)
} LexemeOpr, LexemeOperation;

// LEXEME_LITERAL_TYPE aka LexemeLitType & LexemeLiteralType
// To represent type of literal that has been encountered in process of finding
// LITERALS_TYPE and later this sahll be used to determine default action based
// on type itself
typedef enum LEXEME_LITERAL_TYPE{
    LITERAL_ADDRESS,                    // [label] -> direct translation to addr
    LITERAL_COMMENT,                    // ;; comments notion
    LITERAL_NUMERIC,                    // 0xblahblah or maybe any number 123
    LITERAL_STRING,                     // "this is a string, HELLO WORLD!"
    LITERAL_NONE,                       // dont ask me why im writing this :-(
    // LITERAL_NONE is suppoesed to be used for both labels and keywords intially
} LexemeLitType, LexemeLiteralType;

// LEXEME_LITERAL aka LexemeLit & LexemeLiteral
// LEXEME_LITERAL is representation of literal types that has been defined within
// assembly implementation, it is important to note that only some literals have
// been defined, if none of literal type match then it is to be considerd as a 
// a lebel, later this label is to be chaecked against keywords, if current lexem
// -e matches to a keyword then in that case it cannot be a label.
//
// -----------------+-----------------------------------------------------------+
// LITERAL_STRING   | "anything between double quotes is a string" or 's'ingle  |
// LITERAL_COMMENT  | ;; anything appearing after ";" is a comment              |
// LITERAL_NUMBER   | anything starting from NUMERIC char is a number           |
// LITERAL_NONE     | if not matched to anything else it doesnt have type  [1]  |
// -----------------+-----------------------------------------------------------+
//
// [1]: when dealing with any literal that is of NONE type you need to check it
//      against keywords and if it is not a part of keyword then consider it to 
//      a label
typedef struct LEXEME_LITERAL{
    enum LEXEME_LITERAL_TYPE type;      // type of literal that we hold
    unsigned int line_no;               // line number associated with this
    unsigned int size_of;               // size of this lexeme in num bytes
    const char* data;                   // data to store (associated char*)
} LexemeLit, LexemeLiteral;

// LEXEME_TOKEN aka LexemeToken or Lexeme
// LEXEME_TOKEN holds type of lexeme and associated information to that lexeme
// 
// [ USAGE ] -------------------------------------------------------------------+
// LEXEME_TOKEN lexeme = __allocate;                                            |
// switch (lexeme.type){                                                        |
//      case LEXEME_PUN: return lexeme.as.pun;                                  |
//      case LEXEME_OPR: return lexeme.as.opr;                                  |
//      case LEXEME_LIT: return lexeme.as.lit;                                  |
// }                                                                            |
// -----------------------------------------------------------------------------+
//
// [ NOTE ] --------------------------------------------------------------------+
// doing lexeme.as.<other> can fail with segmentation fault for lexeme.as.<type>|
// -----------------------------------------------------------------------------+
//
// lexeme token for LEXEME_KEY has not been defined (look at CHANGE_LOG) above
typedef struct LEXEME_TOKEN{
    enum ASSEMBLER_LEXEME_TYPE type;    // holds the type of current lexeme
    union {
        struct LEXEME_PUNCTUATION pun;  // read lexeme as a punctuation
        struct LEXEME_OPERATION   opr;  // read lexeme as a operation
        struct LEXEME_LITERAL     lit;  // read lexeme as a literal
    } as;
} LexemeToken, Lexeme;

// FUNCTIONS_FOR_LEXICAL_ANALYSIS
// following are declarations of all functions for lexical analysis of source
// (temporary)
bool lexer(FILE* source, struct LEXEME_TOKEN* result);

#endif
