#include "lexer.h"
#include "exceptions.h"
#include "logging.h"
#include "isa.h"
#include "fmt.h"

bool check_against_punctuations(char c) {
    return c == ',' || c == ':' || c == '.';
}

bool check_against_literal_start(char c) {
    return c == "\"" || c == "'" || c == ";";
}

bool lexer(FILE* source, struct LEXEME_TOKEN* result) {
    char c;
    while ((c = fgetc(source)) != EOF){
        if (check_against_punctuations(c)) {
            result->type = LEXEME_PUN;
            result->as.pun.line_no = 0; 
            result->as.pun.char_no = 0; 
            result->as.pun.data = c; 

            return true;
        }
    }
    return false;
}
