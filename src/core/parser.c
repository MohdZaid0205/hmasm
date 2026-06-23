#include "parser.h"
#include "exceptions.h"
#include "strdump.h"
#include "isa.h"
#include "pch.h"


// Forward Declarations
bool is_directive   (const char* word);
bool is_block       (const char* word);
bool is_data        (const char* word);
bool parse_directive        (FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt);
bool parse_block            (FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt);
bool parse_data_declaration (FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt);
bool parse_const_declaration(FILE* source, struct STATEMENT* stmt);
bool parse_instruction      (FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt, const struct ASSEMBLER_ISA* isa);
bool peek_token             (FILE* source, struct LEXEME_TOKEN* result);
bool get_next_token         (FILE* source, struct LEXEME_TOKEN* result, bool raw_mode);

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
    #define PARSER_DEBUG_STMT(x) DEBUG("[PARSER] Created statement: %s\n", {}, x)
#else
    #define PARSER_DEBUG_STMT(x)
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

static bool __parse_operand(FILE* source, struct INSTRUCTION_COMPONENT* comp, int line_no, const struct ASSEMBLER_ISA* isa) {
    struct LEXEME_TOKEN tok;
    if (!get_next_token(source, &tok, false)) return false;
    
    if (tok.type == LEXEME_LIT) {
        comp->type = INSTRUCTION_COMPONENT_IMMIDIATE_T;
        comp->as.imm.value = strdup(tok.as.lit.data);
        comp->as.imm.lable = NULL;
    } else if (tok.type == LEXEME_WRD) {
        if (isa && isa->is_register && isa->is_register(tok.as.wrd.data)) {
            comp->type = INSTRUCTION_COMPONENT_REGISTER_T;
            comp->as.reg.name = strdup(tok.as.wrd.data);
            comp->as.reg.size = 0;
        } else {
            comp->type = INSTRUCTION_COMPONENT_IMMIDIATE_T;
            comp->as.imm.lable = strdup(tok.as.wrd.data);
            comp->as.imm.value = NULL;
        }
    } else {
        UNEXPECTED_TOKEN_EXCEPTION("Register or Immediate", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }
    return true;
}

bool parse_instruction(FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt, const struct ASSEMBLER_ISA* isa) {
    stmt->type = STATEMENT_INSTRUCTION_T;
    stmt->as.inst.mnemonic = strdup(kw->as.wrd.data);
    
    stmt->as.inst.rd.type = -1;
    stmt->as.inst.rs1.type = -1;
    stmt->as.inst.rs2.type = -1;
    
    struct INSTRUCTION_COMPONENT* components[3] = {&stmt->as.inst.rd, &stmt->as.inst.rs1, &stmt->as.inst.rs2};
    int current_comp = 0;
    
    struct LEXEME_TOKEN next;
    while (peek_token(source, &next)) {
        int next_line_no = next.type == LEXEME_PUN ? next.as.pun.line_no :
                          next.type == LEXEME_WRD ? next.as.wrd.line_no :
                          next.type == LEXEME_LIT ? next.as.lit.line_no :
                          next.as.opr.line_no;
                          
        if (next_line_no > kw->as.wrd.line_no) break;
        
        if (next.type == LEXEME_PUN && next.as.pun.data == ',') {
            get_next_token(source, &next, false);
            continue;
        }
        
        if (current_comp < 3) {
            if (!__parse_operand(source, components[current_comp], kw->as.wrd.line_no, isa)) return false;
            current_comp++;
        } else {
            get_next_token(source, &next, false);
        }
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

bool parse(FILE* source, struct AST* result, const struct ASSEMBLER_ISA* isa) {
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
                if (!parse_instruction(source, &token, &stmt, isa)) return false;
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
        || CHECK("error")
        || CHECK("register");
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
    else if (CHECK("register")) stmt->as.dir.type = DIRECTIVE_REGISTER_ALIAS_T;

    if (stmt->as.dir.type == DIRECTIVE_REGISTER_ALIAS_T) {
        struct LEXEME_TOKEN tok;
        // Parse <name>
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_WRD) {
            UNEXPECTED_TOKEN_EXCEPTION("Alias Name", "Invalid Token", kw->as.wrd.line_no, kw->as.wrd.char_no);
            return false;
        }
        stmt->as.dir.target = strdup(tok.as.wrd.data);
        
        // Parse :=
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != ':') {
            UNEXPECTED_TOKEN_EXCEPTION(":", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != '=') {
            UNEXPECTED_TOKEN_EXCEPTION("=", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        
        // Parse <reg>
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_WRD) {
            UNEXPECTED_TOKEN_EXCEPTION("Target Register", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        stmt->as.dir.modifier = strdup(tok.as.wrd.data);
        
        // Parse @<arch>
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != '@') {
            UNEXPECTED_TOKEN_EXCEPTION("@", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_WRD) {
            UNEXPECTED_TOKEN_EXCEPTION("Target Architecture", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        stmt->as.dir.arch = strdup(tok.as.wrd.data);
        return true;
    }

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
    stmt->as.dir.arch = NULL;

    return true;
}

bool parse_block(FILE* source, struct LEXEME_TOKEN* keyword, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_BLOCK_T;
    const char* word = keyword->as.wrd.data;
    
    struct LEXEME_TOKEN header_line;
    get_next_token(source, &header_line, true);
    
    char end_marker[256];
    sprintf(end_marker, "%%%s .", word);

    if (CHECK("optimization")) {
        stmt->as.blk.type = BLOCK_OPTIMIZATION_T;
        stmt->as.blk.as.opt.chunk_count = 0;
        stmt->as.blk.as.opt.chunks = malloc(sizeof(struct OPT_CHUNK) * 16);
        stmt->as.blk.data = NULL; // Unused for opt

        char current_arch[256] = {0};
        const char* ptr = header_line.as.lit.data;
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '@') ptr++;
        int i = 0;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r') {
            current_arch[i++] = *ptr++;
        }
        
        int data_cap = 1024;
        int data_len = 0;
        char* current_data = malloc(data_cap);
        current_data[0] = '\0';
        
        struct LEXEME_TOKEN raw_line;
        bool end_found = false;
        
        while (get_next_token(source, &raw_line, true)) {
            if (strncmp(raw_line.as.lit.data, end_marker, strlen(end_marker)) == 0) {
                // save current chunk
                stmt->as.blk.as.opt.chunks[stmt->as.blk.as.opt.chunk_count].arch = strdup(current_arch);
                stmt->as.blk.as.opt.chunks[stmt->as.blk.as.opt.chunk_count].data = current_data;
                stmt->as.blk.as.opt.chunk_count++;
                end_found = true;
                break; 
            }
            
            if (strncmp(raw_line.as.lit.data, "%optimization @", 15) == 0) {
                // Save current chunk
                stmt->as.blk.as.opt.chunks[stmt->as.blk.as.opt.chunk_count].arch = strdup(current_arch);
                stmt->as.blk.as.opt.chunks[stmt->as.blk.as.opt.chunk_count].data = current_data;
                stmt->as.blk.as.opt.chunk_count++;
                
                // Parse new arch
                memset(current_arch, 0, sizeof(current_arch));
                ptr = raw_line.as.lit.data + 15;
                i = 0;
                while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r') {
                    current_arch[i++] = *ptr++;
                }
                
                // Start new chunk
                data_cap = 1024;
                data_len = 0;
                current_data = malloc(data_cap);
                current_data[0] = '\0';
                continue;
            }
            
            int line_len = raw_line.as.lit.size_of;
            if (data_len + line_len + 2 > data_cap) {
                data_cap = data_len + line_len + 1024;
                current_data = realloc(current_data, data_cap);
            }
            strcat(current_data, raw_line.as.lit.data);
            strcat(current_data, "\n");
            data_len += line_len + 1;
        }
        
        if (!end_found) {
            UNEXPECTED_TOKEN_EXCEPTION(end_marker, "EOF", keyword->as.wrd.line_no, keyword->as.wrd.char_no);
            return false;
        }
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
    }
    
    return true;
}

bool parse_data_declaration(FILE* source, struct LEXEME_TOKEN* kw, struct STATEMENT* stmt) {
    stmt->type = STATEMENT_DECLARATION_T;
    struct DECLARATION* decl = &stmt->as.decl;
    
    bool is_reserve = strcmp(kw->as.wrd.data, "reserve") == 0;
    decl->type = is_reserve ? DECLARATION_COMPONENT_RESERVE_T : DECLARATION_COMPONENT_DATA_T;
    
    struct LEXEME_TOKEN tok;

    if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != '@') {
        UNEXPECTED_TOKEN_EXCEPTION("@", "Missing '@'", tok.as.pun.line_no, tok.as.pun.char_no);
        return false;
    }

    if (!get_next_token(source, &tok, false) || tok.type != LEXEME_WRD) {
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

    if (is_reserve) {
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_LIT) {
            UNEXPECTED_TOKEN_EXCEPTION("Reserve Size Literal", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        decl->as.resv.size = atoi(tok.as.lit.data);
    } else {
        if (!get_next_token(source, &tok, false) || tok.type != LEXEME_PUN || tok.as.pun.data != '[') {
            UNEXPECTED_TOKEN_EXCEPTION("[", "Missing '['", tok.as.pun.line_no, tok.as.pun.char_no);
            return false;
        }
        
        int capacity = 8;
        decl->as.vals.values = malloc(sizeof(struct INSTRUCTION_COMPONENT_IMMIDIATE) * capacity);
        decl->as.vals.count = 0;
        
        while (true) {
            if (!get_next_token(source, &tok, false)) return false;
            
            if (tok.type == LEXEME_PUN && tok.as.pun.data == ']') {
                break;
            }
            
            if (tok.type == LEXEME_PUN && tok.as.pun.data == ',') {
                continue; // Skip commas
            }
            
            if (tok.type != LEXEME_LIT && tok.type != LEXEME_WRD) {
                UNEXPECTED_TOKEN_EXCEPTION("Value or ']'", "Invalid Token", tok.as.pun.line_no, tok.as.pun.char_no);
                return false;
            }
            
            if (decl->as.vals.count >= capacity) {
                capacity *= 2;
                decl->as.vals.values = realloc(decl->as.vals.values, sizeof(struct INSTRUCTION_COMPONENT_IMMIDIATE) * capacity);
            }
            
            decl->as.vals.values[decl->as.vals.count].value = strdup(tok.type == LEXEME_LIT ? tok.as.lit.data : tok.as.wrd.data);
            decl->as.vals.values[decl->as.vals.count].lable = NULL;
            decl->as.vals.count++;
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
