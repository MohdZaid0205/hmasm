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
// FIXME: just revamp how we are to deal with chrachters and collection process

bool check_against_literal_start(char c) {
    return c == "\"" || c == "'" || c == ";";
}

bool lexer(FILE* source, struct LEXEME_TOKEN* result) {
    char c;
    while ((c = fgetc(source)) != EOF) {
        if (c == upto) break;
        if (c == '\n') break;    // FIXME: implement error handling
    }
    long int e = ftell(source);
    long int size = e - s;

    char* buffer = malloc(sizeof(char)*size);
    fseek(source, s, SEEK_SET);
    fgets(buffer, size, source);
    fseek(source, e, SEEK_SET);
    
    return buffer;
}

bool lexer(FILE* source, struct LEXEME_TOKEN* result) {
    char p = EOF;
    char c = EOF;
    while ((c = fgetc(source)) != EOF){
        if (check_against_punctuations(c)) {
            result->type = LEXEME_PUN;
            result->as.pun.line_no = 0; 
            result->as.pun.char_no = 0; 
            result->as.pun.data = c; 

            return true;
        }
        p = c;
        
        if (lexer_found_token) return true;
        // else continue;
    }
    return false;
}
