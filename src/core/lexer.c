#include "lexer.h"
#include "exceptions.h"
#include "strdump.h"

// WARNINGS and EXCEPTIONS along with DEBUG statements as per requirement

#define LEXER_DEBUG_PUNCTUATION(p)                                              \
    DEBUG(LEXER_DEBUG_PUN, {}, p->line_no, p->char_no, p->data)

#define LEXER_DEBUG_OPERATION(o)                                                \
    DEBUG(LEXER_DEBUG_OPR, {}, o->line_no, o->char_no, o->data)

#define LEXER_DEBUG_LITERAL(l)                                                  \
    DEBUG(LEXER_DEBUG_LIT, {}, l->line_no, l->char_no, l->size_of, l->data)

#define LEXER_DEBUG_WORD(w)                                                     \
    DEBUG(LEXER_DEBUG_WRD, {}, w->line_no, w->char_no, w->size_of, w->data)


// UNNESSASARY but USEFUL macros

#define CHECK(w) c == w
#define PREVS(w) p == w

// CATEGORIZATION of charachter - token helper functions

bool __check_against_ign(char c) {
    return CHECK(' ')
        || CHECK('\n');
}

bool __check_against_pun(char c) {
    return CHECK(',') 
        || CHECK(':') 
        || CHECK('.')
        || CHECK('%');
}

bool __check_against_opr(char c) {
    return CHECK('&')
        || CHECK('*')
        || CHECK('+')
        || CHECK('-');
}

bool __check_against_lit(char c) {
    return CHECK('"')
        || CHECK('0')
        || CHECK(';')
        || CHECK('#');
}

bool __check_against_wrd(char c) {
    return __check_against_opr(c)
        || __check_against_pun(c)
        || __check_against_ign(c);
}

