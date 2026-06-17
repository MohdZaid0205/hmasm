#include "parser.h"
#include "exceptions.h"
#include "strdump.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// MACROS for error reporting
#define UNEXPECTED_TOKEN_EXCEPTION(expected, actual, lno, cno)                  \
    EXCEPTION(PARSER_UTE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_UTE_LN1, actual, expected);                  \
        EXCEPTION_LN(INSET, PARSER_UTE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_UTE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_UTE_END, actual);                                   \
    }, actual)

#define INVALID_DIRECTIVE_EXCEPTION(keyword, lno, cno)                          \
    EXCEPTION(PARSER_IDE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_IDE_LN1, keyword);                           \
        EXCEPTION_LN(INSET, PARSER_IDE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_IDE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_IDE_END, keyword);                                  \
    }, keyword)

#define INVALID_DECLARATION_EXCEPTION(context, lno, cno)                        \
    EXCEPTION(PARSER_DCL_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_DCL_LN1);                                    \
        EXCEPTION_LN(INSET, PARSER_DCL_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_DCL_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_DCL_END, context);                                  \
    }, context)

#define INVALID_INSTRUCTION_EXCEPTION(mnemonic, lno, cno)                       \
    EXCEPTION(PARSER_IIE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_IIE_LN1, mnemonic);                          \
        EXCEPTION_LN(INSET, PARSER_IIE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_IIE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_IIE_END, mnemonic);                                 \
    }, mnemonic)

#ifdef PARSER_DEBUG
    // TODO: add debug statements
#endif


// Token Lookahead State
static struct LEXEME_TOKEN __lookahead;
static bool __has_lookahead = false;

// Safely fetches next token, using cached lookahead if available
bool get_next_token(FILE* source, struct LEXEME_TOKEN* result, bool raw_mode) {
    if (__has_lookahead && !raw_mode) {
        *result = __lookahead;
        __has_lookahead = false;
        return true;
    }
    return lexer(source, result, raw_mode);
}

bool parse_instruction(FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_INSTRUCTION_T;
    
    // Read operands until the next token is on a new line
    struct LEXEME_TOKEN next;
    while (peek_token(source, &next)) {
        // If the next token is on a different line, the instruction is over!
        if (next.type == LEXEME_PUN && next.as.pun.line_no > kw->as.wrd.line_no) break;
        if (next.type == LEXEME_WRD && next.as.wrd.line_no > kw->as.wrd.line_no) break;
        if (next.type == LEXEME_LIT && next.as.lit.line_no > kw->as.wrd.line_no) break;
        if (next.type == LEXEME_OPR && next.as.opr.line_no > kw->as.wrd.line_no) break;
        
        get_next_token(source, &next, false);
    }
    
    return true;
}

// Peeks at the next token without consuming it
bool peek_token(FILE* source, struct LEXEME_TOKEN* result) {
    if (!__has_lookahead) {
        __has_lookahead = lexer(source, &__lookahead, false);
    }
    if (__has_lookahead) {
        *result = __lookahead;
        return true;
    }
    return false;
}

bool parse(FILE* source, struct AST* result) {
    result->count = 0;
    result->capacity = 16;
    result->statements = malloc(sizeof(struct STATEMENT) * result->capacity);
    
    struct LEXEME_TOKEN token;
    while (get_next_token(source, &token, false)) {
        
        if (token.type == LEXEME_LIT && token.as.lit.type == LITERAL_COMMENT) continue;
        
        struct STATEMENT stmt;
        
        // BRANCH 1: % (Directives, Blocks, Data Arrays/Reserve)
        if (token.type == LEXEME_PUN && token.as.pun.data == '%') {
            struct LEXEME_TOKEN kw;
            if (!get_next_token(source, &kw, false) || kw.type != LEXEME_WRD) {
                UNEXPECTED_TOKEN_EXCEPTION("WORD (Directive Name)", "something else", token.as.pun.line_no, token.as.pun.char_no);
                return false; 
            }
            
            if (is_directive(kw.as.wrd.data)) {
                if (!parse_directive(source, &kw, &stmt)) return false;
            } else if (is_block(kw.as.wrd.data)) {
                if (!parse_block(source, &kw, &stmt)) return false;
            } else if (is_data(kw.as.wrd.data)) {
                if (!parse_data_declaration(source, &kw, &stmt)) return false;
            } else {
                INVALID_DIRECTIVE_EXCEPTION(kw.as.wrd.data, kw.as.wrd.line_no, kw.as.wrd.char_no);
                return false;
            }
        } 
        // BRANCH 2: @ (Constants)
        else if (token.type == LEXEME_PUN && token.as.pun.data == '@') {
            if (!parse_const_declaration(source, &stmt)) return false;
        }
        // BRANCH 3: Words (Instructions or Labels)
        else if (token.type == LEXEME_WRD) {
            struct LEXEME_TOKEN next;
            if (peek_token(source, &next) && next.type == LEXEME_PUN && next.as.pun.data == ':') {
                get_next_token(source, &next, false); // Consume the ':'
                stmt.type = STATEMENT_LABEL_T;
                stmt.as.lable = strdup(token.as.wrd.data);
            } else {
                if (!parse_instruction(source, &token, &stmt)) return false;
            }
        }
        else {
            UNEXPECTED_TOKEN_EXCEPTION("Start of statement (%, @, or Word)", "Invalid Token", 0, 0);
            return false;
        }
        
        if (result->count >= result->capacity) {
            result->capacity *= 2;
            result->statements = realloc(result->statements, sizeof(struct STATEMENT) * result->capacity);
        }
        result->statements[result->count++] = stmt;
    }
    return true;
}

// Helpers
#define CHECK(str) strcmp(word, str) == 0

bool is_directive(const char* word) {
    return CHECK("section")
        || CHECK("align")
        || CHECK("global")
        || CHECK("extern")
        || CHECK("entry")
        || CHECK("include")
        || CHECK("error");
}

bool is_block(const char* word) {
    return CHECK("optimization")
        || CHECK("macro");
}
