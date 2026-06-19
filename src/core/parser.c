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

bool is_data(const char* word) {
    return CHECK("data")
        || CHECK("reserve");
}

bool parse_directive(FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_DIRECTIVE_T;
    const char* word = kw->as.wrd.data;
    
    if      (CHECK("section")) stmt->as.dir.type = DIRECTIVE_SECTION_T;
    else if (CHECK("align"))   stmt->as.dir.type = DIRECTIVE_ALIGN_T;
    else if (CHECK("global"))  stmt->as.dir.type = DIRECTIVE_GLOBAL_T;
    else if (CHECK("extern"))  stmt->as.dir.type = DIRECTIVE_EXTERN_T;
    else if (CHECK("entry"))   stmt->as.dir.type = DIRECTIVE_ENTRY_T;
    else if (CHECK("include")) stmt->as.dir.type = DIRECTIVE_INCLUDE_T;
    else if (CHECK("error"))   stmt->as.dir.type = DIRECTIVE_ERROR_T;

    struct LEXEME_TOKEN target;
    if (!get_next_token(source, &target, false)) {
        UNEXPECTED_TOKEN_EXCEPTION("Target Argument", "EOF", kw->as.wrd.line_no, kw->as.wrd.char_no);
        return false;
    }
    
    if (target.type == LEXEME_WRD || target.type == LEXEME_LIT) {
        stmt->as.dir.target = target.type == LEXEME_WRD ? (char*)target.as.wrd.data : (char*)target.as.lit.data;
    } else if (target.type == LEXEME_PUN && target.as.pun.data == '.') {
        struct LEXEME_TOKEN target_word;
        if (!get_next_token(source, &target_word, false)) return false;
        char buf[256];
        sprintf(buf, ".%s", target_word.as.wrd.data);
        stmt->as.dir.target = strdup(buf);
    } else {
        stmt->as.dir.target = NULL;
    }
    
    stmt->as.dir.modifier = NULL;

    return true;
}

bool parse_block(FILE* source, struct LEXEME_TOKEN* keyword, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_BLOCK_T;
    const char* word = keyword->as.wrd.data;
    
    struct LEXEME_TOKEN header_line;
    get_next_token(source, &header_line, true);
    
    if (CHECK("optimization")) {
        stmt->as.blk.type = BLOCK_OPTIMIZATION_T;
        char arch_name[256] = {0};
        const char* ptr = header_line.as.lit.data;
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '@') ptr++;
        int i = 0;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r') {
            arch_name[i++] = *ptr++;
        }
        stmt->as.blk.as.opt.arch = strdup(arch_name);
    } else if (CHECK("macro")) {
        stmt->as.blk.type = BLOCK_MACRO_T;
        // Parse the macro name
        char mac_name[256] = {0};
        const char* ptr = header_line.as.lit.data;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        int i = 0;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && *ptr != '.') {
            mac_name[i++] = *ptr++;
        }
        stmt->as.blk.as.mac.name = strdup(mac_name);
    }
    
    char end_marker[256];
    sprintf(end_marker, "%%%s .", word);
    
    int data_cap = 1024;
    int data_len = 0;
    stmt->as.blk.data = malloc(data_cap);
    stmt->as.blk.data[0] = '\0';
    
    struct LEXEME_TOKEN raw_line;
    bool end_found = false;
    
    while (get_next_token(source, &raw_line, true)) {
        if (strncmp(raw_line.as.lit.data, end_marker, strlen(end_marker)) == 0) {
            end_found = true;
            break; 
        }
        
        int line_len = raw_line.as.lit.size_of;
        if (data_len + line_len + 2 > data_cap) {
            data_cap *= 2;
            stmt->as.blk.data = realloc(stmt->as.blk.data, data_cap);
        }
        strcat(stmt->as.blk.data, raw_line.as.lit.data);
        strcat(stmt->as.blk.data, "\n");
        data_len += line_len + 1;
    }
    
    if (!end_found) {
        UNEXPECTED_TOKEN_EXCEPTION(end_marker, "EOF", keyword->as.wrd.line_no, keyword->as.wrd.char_no);
        return false;
    }
    return true;
}

bool parse_data_declaration(FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_DECLARATION_T;
    
    // consume @, type, name, =, [, ... ]
    struct LEXEME_TOKEN tok;
    while (get_next_token(source, &tok, false)) {
        if (tok.type == LEXEME_PUN && tok.as.pun.data == ']') {
            break;
        }
    }
    return true;
}

bool parse_const_declaration(FILE* source, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_DECLARATION_T;
    struct DECLARATION* decl = &stmt->as.decl;
    decl->type = DECLARATION_COMPONENT_CONST_T;
    
    struct LEXEME_TOKEN tok;

    if (!get_next_token(source, &tok, false)) return false;

    if (tok.type != LEXEME_WRD) {
        UNEXPECTED_TOKEN_EXCEPTION("DataType (e.g., B, W, D)", "Non-Word Token", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }
    
    char modifier = tok.as.wrd.data[0];
    switch (modifier) {
        case 'B': decl->dt = DECLARATION_B_T; break;
        case 'W': decl->dt = DECLARATION_W_T; break;
        case 'D': decl->dt = DECLARATION_D_T; break;
        case 'Q': decl->dt = DECLARATION_Q_T; break;
        case 'T': decl->dt = DECLARATION_T_T; break;
        case 'Y': decl->dt = DECLARATION_Y_T; break;
        case 'Z': decl->dt = DECLARATION_Z_T; break;
        default: 
            UNEXPECTED_TOKEN_EXCEPTION("Valid DataType (B, W, D, Q, T, Y, Z)", tok.as.wrd.data, tok.as.wrd.line_no, tok.as.wrd.char_no);
            return false;
    }
    
    if (!get_next_token(source, &tok, false) || tok.type != LEXEME_WRD) {
        UNEXPECTED_TOKEN_EXCEPTION("Identifier", "Non-Word Token", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }
    decl->name = strdup(tok.as.wrd.data);
    
    if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != '=') {
        UNEXPECTED_TOKEN_EXCEPTION("=", "Missing '='", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }
    
    if (!get_next_token(source, &tok, false) || (tok.type != LEXEME_LIT && tok.type != LEXEME_WRD)) {
        UNEXPECTED_TOKEN_EXCEPTION("Immediate Value", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }
    decl->as.cnst.value.value = strdup(tok.type == LEXEME_LIT ? tok.as.lit.data : tok.as.wrd.data);
    decl->as.cnst.value.lable = NULL;
    
    return true;
}
