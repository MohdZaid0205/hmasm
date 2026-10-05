#include "lexer.h"
#include "logging.h"
#include "exceptions.h"
#include "strdump.h"
#include <stdio.h>

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
#define CHECKC(v) (c == v)
#define CHECKS(v) (strncmp(v, s, strlen(v)) == 0)
#define PROPER(v) (s[strlen(v)] == ' '          \
                || s[strlen(v)] == '\n'         \
                || s[strlen(v)] == '\t'         \
                || s[strlen(v)] == '\0'         \
                || s[strlen(v)] == EOF          \
        )

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

// utility function for deciding begining
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
        is_key |= CHECKS(keywords[i]) && PROPER(keywords[i]);
    for (size_t i=0; i<sizeof(isawords)/sizeof(isawords[0]); i++)
        is_key |= CHECKS(isawords[i]) && PROPER(isawords[i]);
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

// retreive required part of content from file
bool _collect_idn(struct LEXER* lexer, struct LEXEME* lexeme) {
    // assuming we have checked that upcoming token is supposedly a identifier
    char c;
    long b = ftell(lexer->file);
    while ((c = fgetc(lexer->file)) != EOF) {
        if (!isalnum(c) && !CHECKC('_')) {
            break;
        }
    }
    long e = ftell(lexer->file);
    lexeme->type = LEXEME_IDN;
    lexeme->as.idn.size = e-b-1;
    lexeme->as.idn.data = malloc(sizeof(char)*(e-b));
    
    if (!lexeme->as.idn.data)
        return false;

    fseek(lexer->file, b, SEEK_SET);
    fread(lexeme->as.idn.data, 1, e-b-1, lexer->file);
    lexeme->as.idn.data[e-b-1] = '\0';
    return true;
}
bool _collect_pun(struct LEXER* lexer, struct LEXEME* lexeme) {
    // assuming we have checked that upcoming token is supposedly a punctuation
    lexeme->type = LEXEME_PUN;
    lexeme->as.pun.data = fgetc(lexer->file);
    return true;
}
bool _collect_key(struct LEXER* lexer, struct LEXEME* lexeme) {
    // assuming we have checked that upcoming token is supposedly a keyword
    
    char  c = EOF; /**insignificant*/
    char* s = lexer->look_ahead_buff;
    
    char* key = NULL;
    for (size_t i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++)
        if (CHECKS(keywords[i]) && PROPER(keywords[i]))
            key = (char*) keywords[i];
    for (size_t i=0; i<sizeof(isawords)/sizeof(isawords[0]); i++)
        if (CHECKS(isawords[i]) && PROPER(isawords[i]))
            key = (char*) isawords[i];
    lexeme->type = LEXEME_KEY;
    lexeme->as.key.data = key;
    lexeme->as.key.size = sizeof(key);
    return true;
}
bool _collect_lit(struct LEXER* lexer, struct LEXEME* lexeme);
    // TODO: imeplement required supportive functions
    //      TODO: _collect_lit_string
    //      TODO: _collect_lit_comment
    //          TODO: -> _inline
    //          TODO: -> _outline
    //      TODO: _collect_lit_numeric
    //          TODO: -> _binary  | _2
    //          TODO: -> _hexa    | _8
    //          TODO: -> _decimal | _10
    //          TODO: -> _octal   | _16
    // NOTE: i may as well treat arrays as literals
bool _collect_opr(struct LEXER* lexer, struct LEXEME* lexeme) {
    // assuming we have checked that upcoming token is supposedly a operation
    lexeme->type = LEXEME_OPR;
    lexeme->as.opr.data = fgetc(lexer->file);
    return true;
}
bool _collect_nul(struct LEXER* lexer, struct LEXEME* lexeme) {
    // assuming we have checked that upcoming token is unclassified
    lexeme->type = LEXEME_NUL;
    lexeme->as.nul.data = fgetc(lexer->file);
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

    lexer->look_ahead_buff = malloc(sizeof(char)*(lexer->look_ahead_size + 2));
    if (lexer->look_ahead_buff == NULL)
        lexer->look_ahead_size = 0;
    
    return lexer->look_ahead_size > 0;
}

// LEXER implementation
bool lexer_next(struct LEXER* lexer, struct LEXEME* lexeme) {
    char c = fgetc(lexer->file);
    if (c == EOF)
        return false;

    if (_begin_idn(c, lexer->look_ahead_buff)) {
        fseek(lexer->file, -1, SEEK_CUR);
        _collect_idn(lexer, lexeme);
    } else {
        lexeme->type = LEXEME_NUL;
        lexeme->as.nul.data = c;
    }
    _print_lexeme(lexeme);
    return true;
}
