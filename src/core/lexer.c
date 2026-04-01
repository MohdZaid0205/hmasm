#include "lexer.h"
#include "exceptions.h"
#include "logging.h"
#include "isa.h"
#include "fmt.h"
#include "strdump.h"

void __print_lexeme_punctuation(struct LEXEME_PUNCTUATION* p) {
    DBG("%[ LEXER %] found %(LEXEME_PUN('%c')%) at %d:%d\n",
        p->data, p->line_no, p->char_no );
}

void __print_lexeme_operation(struct LEXEME_OPERATION* o) {
    DBG("%[ LEXER %] found %(LEXEME_OPR('%c')%) at %d:%d\n",
        o->data, o->line_no, o->char_no );
}

void __print_lexeme_literal(struct LEXEME_LITERAL* l) {
    char* __literal_type_string;
    switch (l->type) {
        case LITERAL_ADDRESS: __literal_type_string = "LITERAL_ADDRESS"; break;
        case LITERAL_COMMENT: __literal_type_string = "LITERAL_COMMENT"; break;
        case LITERAL_NUMERIC: __literal_type_string = "LITERAL_NUMERIC"; break;
        case LITERAL_STRING : __literal_type_string = "LITERAL_STRING" ; break;
        case LITERAL_NONE   : __literal_type_string = "LITERAL_NONE"   ; break;
    };

    DBG("%[ LEXER %] found %(LEXEME_LIT( type:%s )%) at %d:%d with value=%(%s%)\n",
        __literal_type_string, l->line_no, l->size_of, l->data);
}

void __print_lexeme_word(struct LEXEME_WORD* w){
    DBG("%[ LEXER %] found %(LEXEME_WRD(%s)%) at %d:%d\n",
        w->data, w->line_no, w->size_of);
}
// FIXME: just revamp how we are to deal with chrachters and collection process

#define CHECK(w) c == w
#define PREVS(w) p == w

bool check_against_ign(char c) {
    return CHECK(' ')
        || CHECK('\n');
}

bool check_against_pun(char c) {
    return CHECK(',') 
        || CHECK(':') 
        || CHECK('.')
        || CHECK('%');
}

bool check_against_opr(char c) {
    return CHECK('&') 
        || CHECK('*');
}

bool check_against_lit(char c) {
    return CHECK('"')
        || CHECK('0')
        || CHECK('\'') 
        || CHECK('#');
}

bool check_against_wrd(char c) {
    return ( check_against_opr(c) 
            ||check_against_pun(c)
            ||check_against_ign(c)
    );
}


char* collect_upto(FILE* source, char upto) {
    long int s = ftell(source) - 1;
    char c;
    while ((c = fgetc(source)) != EOF) {
        if (c == upto) break;
        if (c == '\n') break;    // FIXME: implement error handling
    }
    long int e = ftell(source);
    long int size = e - s + 1;

    char* buffer = malloc(sizeof(char)*size);
    fseek(source, s, SEEK_SET);
    if (c == ' ' || c == '\n'){
        fgets(buffer, size - 1, source);
        fseek(source, e - 1, SEEK_SET);
    } else {
        fgets(buffer, size, source);
    }
    return buffer;
}

char* collect_till(FILE* source, bool (*cond)(char)){
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


bool lexer(FILE* source, struct LEXEME_TOKEN* result) {
    char p = EOF;
    char c = EOF;
    while ((c = fgetc(source)) != EOF){
        bool lexer_found_token = false;
        if (check_against_ign(c)) {
            if (c == '\n') lfprintf(LEVEL_ERR, stderr, "\\N");
            lfprintf(LEVEL_ERR, stderr, "%c", c);
            // FIXME: may require some sking of behaviour
        } else if (check_against_pun(c)) {
            lfprintf(LEVEL_ERR, stderr, "%[%c%]", c);
            result->type = LEXEME_PUN;
            result->as.pun.line_no = 0; 
            result->as.pun.char_no = 0; 
            result->as.pun.data = c; 
            __print_lexeme_punctuation(&result->as.pun);
            lexer_found_token = true;
        } else if (check_against_opr(c)) {
            lfprintf(LEVEL_ERR, stderr, "%[%c%]", c);
            result->type = LEXEME_OPR;
            result->as.opr.line_no = 0;
            result->as.opr.char_no = 0;
            result->as.opr.data = c;
            __print_lexeme_operation(&result->as.opr);
            lexer_found_token = true;
        } else if (check_against_lit(c) && (
                PREVS(' ') || PREVS('\n') || PREVS(EOF)
            )) {
            char e = CHECK('#') ? '\n' : CHECK('0') ? ' ' : c;
            switch (c) {
                case '"': result->as.lit.type = LITERAL_STRING ; break;
                case '0': result->as.lit.type = LITERAL_NUMERIC; break;
                case '#': result->as.lit.type = LITERAL_COMMENT; break;
                default : result->as.lit.type = LITERAL_NONE   ; break;
            };
            result->type = LEXEME_LIT;
            result->as.lit.line_no = 0;
            result->as.lit.size_of = 0;
            result->as.lit.data = collect_upto(source, e);
            lfprintf(LEVEL_WRN, stderr, "%[%s%]", result->as.lit.data);
            __print_lexeme_literal(&result->as.lit);
            lexer_found_token = true;
        } else {
            // lfprintf(LEVEL_ERR, stderr, "%[%c%]", c);
            result->type = LEXEME_WRD;
            result->as.wrd.line_no = 0;
            result->as.wrd.size_of = 0;
            result->as.wrd.data = collect_till(source, check_against_wrd);
            lfprintf(LEVEL_INF, stderr, "%[%s%]", result->as.wrd.data);
            __print_lexeme_word(&result->as.wrd);
            lexer_found_token = true;
        }
        p = c;
        
        if (lexer_found_token) return true;
        // else continue;
    }
    return false;
}
