#include "lexer.h"
#include "logging.h"
#include "exceptions.h"
#include "strdump.h"

// Implemetaion for _print_lexeme_[...]
void _print_lexeme_idn(struct LEXEME_IDENTIFIER *idn) {
    DEBUG(LEXER_PRINT_IDN, {}, LEXER_PRINT_ARG_STR(idn));
}
void _print_lexeme_pun(struct LEXEME_PUNCTUATION *pun) {
    DEBUG(LEXER_PRINT_PUN, {}, LEXER_PRINT_ARG_CHR(pun));
}
void _print_lexeme_key(struct LEXEME_KEYWORD* key) {
    DEBUG(LEXER_PRINT_KEY, {}, LEXER_PRINT_ARG_STR(key));
}
void _print_lexeme_lit(struct LEXEME_LITERAL* lit) {
    DEBUG(LEXER_PRINT_LIT, {}, LEXER_PRINT_ARG_STR(lit));
}
void _print_lexeme_opr(struct LEXEME_OPERATION* opr) {
    DEBUG(LEXER_PRINT_OPR, {}, LEXER_PRINT_ARG_CHR(opr));
}
void _print_lexeme_nul(struct LEXEME_NULL* nul) {
    DEBUG(LEXER_PRINT_NUL, {}, LEXER_PRINT_ARG_CHR(nul));
}

// Implementaion for print_lexeme
void _print_lexeme(struct LEXEME *lex) {
    switch (lex->type) {
        case LEXEME_IDN:
            _print_lexeme_idn(&lex->as.idn);
            break;
        case LEXEME_PUN:
            _print_lexeme_pun(&lex->as.pun);
            break;
        case LEXEME_KEY:
            _print_lexeme_key(&lex->as.key);
            break;
        case LEXEME_LIT:
            _print_lexeme_lit(&lex->as.lit);
            break;
        case LEXEME_OPR:
            _print_lexeme_opr(&lex->as.opr);
            break;
        case LEXEME_NUL:
            _print_lexeme_nul(&lex->as.nul);
            break;
        default:
            EXCEPTION(LEXER_PRINT_UKN, {
                EXCEPTION_LN(LEXER_PRINT_UKN_INF, "");
                EXCEPTION_EN(LEXER_PRINT_UKN_END, TFILE, TLINE);
            }, "");
            break;
    }
}

// useless definitions to make things cleaner
#define INCREMENT(x) x += 1
#define DECREMENT(x) x -= 1
#define CHECKC(v) c == v
#define CHECKS(v) strncmp(v, s, strlen(v)) == 0

// some pre-defined keywords
static const char* keywords[] = {
    "align",
    "const",
    "data",
    "datatype",
    "endmacro",
    "entry",
    "extern",
    "global",
    "include",
    "macro",
    "optimization",
    "register",
    "reserve",
    "section",
};

// some pre-defined instructions
static const char* isawords[] = {
    "mov", "push", "pop",
    "inc", "dec" ,
    "add", "sub" , "mul", "div",
    "and", "or"  , "xor", "not",
    "shl", "shr" ,
    "beq", "bneq",
    "bg" , "bgeq",
    "bs" , "bseq",
    "jmp", "call",
    "nop", "hlt" ,
    "int", "sti" , "cli",
};

// utility functions for lexer
bool _begin_idn(char c, char* s) {
    return isalpha(c)
        || CHECKC('_');
}
bool _begin_pun(char c, char* s) {
    return CHECKC('%')
        || CHECKC(':')
        || CHECKC('[')
        || CHECKC(']')
        || CHECKC('(')
        || CHECKC(')')
        || CHECKC('@')
        || CHECKC(',')
        || CHECKC('.');
}
bool _begin_key(char c, char* s) {
    bool is_key = false;
    for (size_t i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++)
        is_key |= CHECKS(keywords[i]);
    for (size_t i=0; i<sizeof(isawords)/sizeof(isawords[0]); i++)
        is_key |= CHECKS(isawords[i]);
    return is_key;
}
bool _begin_lit(char c, char* s) {
    return CHECKC('"')
        || CHECKC(';')
        || isdigit(c);
}
bool _begin_opr(char c, char* s) {
    return CHECKC('=')
        || CHECKC('*')
        || CHECKC('-');
}
bool _begin_nul(char c, char* s) {
    return true;
}

// LEXER look_ahead Implementaion
bool lexer_look(struct LEXER* lexer) {

    lexer->look_ahead_size = 1;
    for (size_t i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++)
        lexer->look_ahead_size = lexer->look_ahead_size > strlen(keywords[i])
            ? lexer->look_ahead_size : strlen(keywords[i]);
    for (size_t i=0; i<sizeof(isawords)/sizeof(isawords[0]); i++)
        lexer->look_ahead_size = lexer->look_ahead_size > strlen(isawords[i])
            ? lexer->look_ahead_size : strlen(isawords[i]);

    lexer->look_ahead_buff = malloc(sizeof(char)*lexer->look_ahead_size);
    if (lexer->look_ahead_buff == NULL)
        lexer->look_ahead_size = 0;
    return lexer->look_ahead_size > 0;
}

// LEXER Implementaion
bool lexer_next(struct LEXER* lexer, struct LEXEME* into) {
    return false;
}
