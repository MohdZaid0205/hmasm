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

// LEXER Implementaion
bool lexer_next(struct LEXER* lexer, struct LEXEME* lexeme) {
    char c = EOF;

    // ignore uncessary charachters
    while ((c = fgetc(lexer->file)) != EOF) {
        if (CHECKC(' ')){
            lexer->c_no+= 1;
        } else
        if (CHECKC('\n')){
            lexer->l_no+= 1;
            lexer->c_no = 0;
        } else
            break;
    }
    lexer->c_no += 1;

    fseek(lexer->file, -1, SEEK_CUR);
    size_t s = fread(
        lexer->look_ahead_buff, 1, 
        lexer->look_ahead_size, lexer->file
    );
    lexer->look_ahead_buff[s] = '\0';
    fseek(lexer->file, -s, SEEK_CUR);
    fseek(lexer->file, +1, SEEK_CUR);

    // check against punctuations
    if (_begin_pun(c, lexer->look_ahead_buff)) {
        lexeme->type = LEXEME_PUN;
        lexeme->as.pun.l_no = lexer->l_no;
        lexeme->as.pun.c_no = lexer->c_no;
        lexeme->as.pun.data = c;
    } else
    // check against operations
    if (_begin_opr(c, lexer->look_ahead_buff)) {
        lexeme->type = LEXEME_OPR;
        lexeme->as.pun.l_no = lexer->l_no;
        lexeme->as.pun.c_no = lexer->c_no;
        lexeme->as.pun.data = c;
    } else
    // check against literals
    if (_begin_lit(c, lexer->look_ahead_buff)) {
        lexeme->type = LEXEME_LIT;
        long b = ftell(lexer->file) - 1;
        
        // check against comments
        if (CHECKC(';')) {
            lexeme->as.lit.type = LITERAL_COMMENT;
            while ((c = fgetc(lexer->file)) != EOF) {
                if (CHECKC(';') || CHECKC('\n'))
                    break;
            }
        } else 
        // check against strings
        if (CHECKC('"')) {
            lexeme->as.lit.type = LITERAL_STRING;
            while ((c = fgetc(lexer->file)) != EOF) {
                if (CHECKC('"') || CHECKC('\n'))
                    break;
            }
        } else
        // check agains numbers
        if (isdigit(c)) {
            lexeme->as.lit.type = LITERAL_NUMERIC;
            while ((c = fgetc(lexer->file)) != EOF) {
                if (!isdigit(c))
                    break;
            }
        }

        long e = ftell(lexer->file);
        lexeme->as.lit.size = lexeme->as.lit.type == LITERAL_NUMERIC? e-b-1: e-b;
        lexeme->as.lit.data = malloc(sizeof(char)*(e-b+1));

        fseek(lexer->file, b, SEEK_SET);
        fread(lexeme->as.lit.data, 1, lexeme->as.lit.size, lexer->file);
        lexeme->as.lit.data[e-b] = '\0';
        lexeme->as.lit.l_no = lexer->l_no;
        lexeme->as.lit.c_no = lexer->c_no;

        lexer->c_no += lexeme->as.lit.size;
        
        if (CHECKC('\n') && lexeme->as.lit.type != LITERAL_NUMERIC) {
            lexeme->as.lit.data[e-b-1] = '\0';
            lexer->l_no+= 1;
            lexer->c_no = 0;
        }

    } else
    // check against keywords
    if (_begin_key(c, lexer->look_ahead_buff)) {
        lexeme->type = LEXEME_KEY;
        
        lexeme->as.key.data = NULL;
        char* s = lexer->look_ahead_buff;
        for (size_t i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++)
            if (CHECKS(keywords[i]) && PROPER(keywords[i]))
                lexeme->as.key.data = (char*) keywords[i];
        for (size_t i=0; i<sizeof(isawords)/sizeof(isawords[0]); i++)
            if (CHECKS(isawords[i]) && PROPER(isawords[i]))
                lexeme->as.key.data = (char*) isawords[i];
        lexeme->as.key.size = strlen(lexeme->as.key.data);
        lexeme->as.key.l_no = lexer->l_no;
        lexeme->as.key.c_no = lexer->c_no;
        
        lexer->c_no += lexeme->as.key.size;
        fseek(lexer->file, lexeme->as.key.size - 1, SEEK_CUR);

    } else
    // cehck against identifiers
    if (_begin_idn(c, lexer->look_ahead_buff)) {
        lexeme->type = LEXEME_IDN;
        
        long b = ftell(lexer->file) - 1;
        while ((c = fgetc(lexer->file)) != EOF) {
            if (!isalnum(c) && !(CHECKC('_')))
                break;
        }
        
        long e = ftell(lexer->file);
        lexeme->as.idn.data = malloc(sizeof(char)*(e-b+1));
        lexeme->as.idn.size = e-b-1;

        fseek(lexer->file, b, SEEK_SET);
        fread(lexeme->as.idn.data, 1, lexeme->as.idn.size, lexer->file);
        lexeme->as.idn.data[e-b] = '\0';
        lexeme->as.idn.l_no = lexer->l_no;
        lexeme->as.idn.c_no = lexer->c_no;

        lexer->c_no += lexeme->as.idn.size;
        
        if (CHECKC('\n')) {
            lexeme->as.idn.data[e-b-1] = '\0';
            lexer->l_no+= 1;
            lexer->c_no = 0;
        }   
    }
    else {
        lexeme->type = LEXEME_NUL;
        lexeme->as.nul.l_no = lexer->l_no;
        lexeme->as.nul.c_no = lexer->c_no;
        lexeme->as.nul.data = c;
    }

    // printf("%c", c);

    return c != EOF;
}
