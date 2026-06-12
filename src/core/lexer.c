#include "lexer.h"
#include "exceptions.h"
#include "strdump.h"
#include <ctype.h>

// WARNINGS and EXCEPTIONS along with DEBUG statements as per requirement

#ifdef LEXER_DEBUG
    #define LEXER_DEBUG_PUNCTUATION(p)                                          \
        DEBUG(LEXER_DEBUG_PUN, {}, p.line_no, p.char_no, p.data)

    #define LEXER_DEBUG_OPERATION(o)                                            \
        DEBUG(LEXER_DEBUG_OPR, {}, o.line_no, o.char_no, o.data)

    #define LEXER_DEBUG_LITERAL(l)                                              \
        DEBUG(LEXER_DEBUG_LIT, {}, l.line_no, l.char_no, l.size_of, l.data)

    #define LEXER_DEBUG_WORD(w)                                                 \
        DEBUG(LEXER_DEBUG_WRD, {}, w.line_no, w.char_no, w.size_of, w.data)
#else
    #define LEXER_DEBUG_PUNCTUATION(x)  // LEXER_DEBUG_PUNCTUATION(x)
    #define LEXER_DEBUG_OPERATION(x)    // LEXER_DEBUG_OPERATION(x)
    #define LEXER_DEBUG_LITERAL(x)      // LEXER_DEBUG_LITERAL(x)
    #define LEXER_DEBUG_WORD(x)         // LEXER_DEBUG_WORD(x)
#endif

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
        || CHECK('%')
        || CHECK('=')
        || CHECK('@')
        || CHECK('[')
        || CHECK(']');
}

bool __check_against_opr(char c) {
    return CHECK('&')
        || CHECK('*')
        || CHECK('+')
        || CHECK('-');
}

bool __check_against_lit(char c) {
    return CHECK('"')
        || CHECK(';')
        || CHECK('#')
        || isdigit(c);
}

bool __check_against_wrd(char c) {
    return __check_against_opr(c)
        || __check_against_pun(c)
        || __check_against_ign(c);
}

// STRING collection functions

char* __collect_upto(FILE* source, char upto, bool consume) {
    long int s = ftell(source) - 1;
    char c;
    while ((c = fgetc(source)) != EOF) {
        if (c == upto) break;
        if (c == '\n') break;
    }

    long int e = ftell(source);
    long int size = e - s + 1;

    char* buffer = malloc(sizeof(char)*size);
    fseek(source, s, SEEK_SET);

    if (!consume){
        fgets(buffer, size - 1, source);
        fseek(source, e - 1, SEEK_SET);
    } else {
        fgets(buffer, size, source);
    }

    return buffer;
}

char* __collect_till(FILE* source, bool (*cond)(char)){
    long int s = ftell(source) - 1;
    fseek(source, s == -1 ? 0 : s, SEEK_SET);
    char c;
    while ((c = fgetc(source)) != EOF) {
        if (cond(c)) break;
    }
    long int e = ftell(source);
    long int size = e - s;

    char* buffer = malloc(sizeof(char)*size);
    fseek(source, s, SEEK_SET);
    fgets(buffer, size, source);
    // seek but dont consume previous charachter
    fseek(source, e - ((size > 1) ? 1 : 0), SEEK_SET);

    return buffer;
}

// HELPER VARIABLES

static int __lno = 0;
static int __cno = 0;

// LEXER implementation

bool lexer(FILE* source, struct LEXEME_TOKEN* result, bool raw_mode) {
    
    if (raw_mode) {
        char c = fgetc(source);
        if (c == EOF) return false;
        
        long int s = ftell(source) - 1;
        fseek(source, s, SEEK_SET);
        
        result->type = LEXEME_LIT;
        result->as.lit.type = LITERAL_RAW;
        result->as.lit.data = __collect_upto(source, '\n', true);
        result->as.lit.line_no = __lno;
        result->as.lit.char_no = __cno;
        result->as.lit.size_of = strlen(result->as.lit.data);
        
        __lno++;
        __cno = 0;
        LEXER_DEBUG_LITERAL(result->as.lit);
        return true;
    }

    char p = EOF;
    char c = EOF;

    while ((c = fgetc(source)) != EOF){
        bool lexer_found_token = false;
        __cno ++;

        if (__check_against_ign(c)) {
            if (CHECK('\n')) {
                __lno++;
                __cno=0;
            }
            // FIXME: Add debug statement for ignorables
        }
        else if (__check_against_pun(c)) {
            result->type = LEXEME_PUN;
            result->as.pun.line_no = __lno;
            result->as.pun.char_no = __cno;
            result->as.pun.data = c;
            LEXER_DEBUG_PUNCTUATION(result->as.pun);
            lexer_found_token = true;
        }
        else if (__check_against_opr(c)) {
            result->type = LEXEME_OPR;
            result->as.opr.line_no = __lno;
            result->as.opr.char_no = __cno;
            result->as.opr.data = c;
            LEXER_DEBUG_OPERATION(result->as.opr);
            lexer_found_token = true;
        }
        else if (__check_against_lit(c)
                &&(PREVS(' ')
                || PREVS('\n')
                || PREVS(EOF)
                || __check_against_pun(p)
                || __check_against_opr(p))
            ){
            
            if (c == '"') {
                result->as.lit.type = LITERAL_STRING;
                result->as.lit.data = __collect_upto(source, '"', true);
            } else if (c == '#' || c == ';') {
                result->as.lit.type = LITERAL_COMMENT;
                result->as.lit.data = __collect_upto(source, '\n', false);
            } else if (isdigit(c)) {
                result->as.lit.type = LITERAL_NUMERIC;
                result->as.lit.data = __collect_till(source, __check_against_wrd);
            } else {
                result->as.lit.type = LITERAL_NONE;
                result->as.lit.data = __collect_till(source, __check_against_wrd);
            }

            result->type = LEXEME_LIT;
            result->as.lit.line_no = __lno;
            result->as.lit.char_no = __cno;
            result->as.lit.size_of = strlen(result->as.lit.data);
            __cno += result->as.lit.size_of - 1;
            LEXER_DEBUG_LITERAL(result->as.lit);
            lexer_found_token = true;
        }
        else {
            result->type = LEXEME_WRD;
            result->as.wrd.data = __collect_till(source, __check_against_wrd);
            result->as.wrd.line_no = __lno;
            result->as.wrd.char_no = __cno;
            result->as.wrd.size_of = strlen(result->as.wrd.data);
            __cno += result->as.wrd.size_of;
            LEXER_DEBUG_WORD(result->as.wrd);
            lexer_found_token = true;
        }
        p = c;

        if (lexer_found_token) return true;
    }

    return false;
}
