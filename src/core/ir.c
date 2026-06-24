#include "ir.h"
#include "parser.h"
#include "exceptions.h"
#include "logging.h"
#include "strdump.h"
#include "pch.h"

void ir_dump_block(struct BLOCK* blk) {
    if (blk->type == BLOCK_OPTIMIZATION_T) {
        DEBUG_LN("", IR_DEBUG_BLK_OPT);
        for (int i = 0; i < blk->as.opt.chunk_count; i++) {
            DEBUG_LN("", IR_DEBUG_BLK_ARC, blk->as.opt.chunks[i].arch);
            DEBUG_LN("", IR_DEBUG_BLK_RAW);
            
            char* data = strdup(blk->as.opt.chunks[i].data);
            char* line = strtok(data, "\n");
            while (line) {
                DEBUG_LN("", IR_DEBUG_BLK_RLN, line);
                line = strtok(NULL, "\n");
            }
            free(data);
        }
    } else if (blk->type == BLOCK_MACRO_T) {
        DEBUG_LN("", IR_DEBUG_BLK_MAC, blk->as.mac.name ? blk->as.mac.name : "UNKNOWN");
        DEBUG_LN("", IR_DEBUG_BLK_RAW);
        
        char* data = strdup(blk->data);
        char* line = strtok(data, "\n");
        while (line) {
            DEBUG_LN("", IR_DEBUG_BLK_RLN, line);
            line = strtok(NULL, "\n");
        }
        free(data);
    }
}

void ir_dump_directive(struct DIRECTIVE* dir) {
    switch (dir->type) {
        case DIRECTIVE_SECTION_T: DEBUG_LN("", IR_DEBUG_DIR_SEC); break;
        case DIRECTIVE_ALIGN_T:   DEBUG_LN("", IR_DEBUG_DIR_ALG); break;
        case DIRECTIVE_GLOBAL_T:  DEBUG_LN("", IR_DEBUG_DIR_GLB); break;
        case DIRECTIVE_EXTERN_T:  DEBUG_LN("", IR_DEBUG_DIR_EXT); break;
        case DIRECTIVE_ENTRY_T:   DEBUG_LN("", IR_DEBUG_DIR_ENT); break;
        case DIRECTIVE_INCLUDE_T: DEBUG_LN("", IR_DEBUG_DIR_INC); break;
        case DIRECTIVE_ERROR_T:   DEBUG_LN("", IR_DEBUG_DIR_ERR); break;
        default:                  DEBUG_LN("", IR_DEBUG_DIR_UNK); break;
    }
    DEBUG_LN("", IR_DEBUG_DIR_TRG, dir->target ? dir->target : "<none>");
    if (dir->modifier) {
        DEBUG_LN("", IR_DEBUG_DIR_MOD, dir->modifier);
    }
}

void ir_dump_declaration(struct DECLARATION* decl) {
    if (decl->type == DECLARATION_COMPONENT_CONST_T) {
        DEBUG_LN("", "  [DECLARATION] CONST %(%s%) = %(%s%)" NLINE, decl->name, decl->as.cnst.value.value);
    } else {
        DEBUG_LN("", IR_DEBUG_DCL);
    }
}

static void __dump_inst_comp(struct INSTRUCTION_COMPONENT* comp) {
    if ((int)comp->type == -1) return; // Uninitialized
    
    if (comp->type == INSTRUCTION_COMPONENT_REGISTER_T) {
        DBG(" REG %(%s%)", comp->as.reg.name);
    } else if (comp->type == INSTRUCTION_COMPONENT_IMMIDIATE_T) {
        if (comp->as.imm.value) {
            DBG(" IMM %(%s%)", comp->as.imm.value);
        } else if (comp->as.imm.lable) {
            DBG(" LBL %(%s%)", comp->as.imm.lable);
        }
    } else if (comp->type == INSTRUCTION_COMPONENT_ADDRESS_T) {
        DBG(" ADR: %(%s(offset %s)%)", comp->as.adr.name, comp->as.adr.offset);
    }
}


void ir_dump_instruction(struct INSTRUCTION* inst) {
    DBG("  INSTRUCTION %(%s%)", inst->mnemonic ? inst->mnemonic : "UNKNOWN");
    __dump_inst_comp(&inst->rd);
    __dump_inst_comp(&inst->rs1);
    __dump_inst_comp(&inst->rs2);
    DBG(NLINE);
}

void ir_dump_statement(struct STATEMENT* stmt) {
    switch (stmt->type) {
        case STATEMENT_LABEL_T:
            DEBUG_LN("", IR_DEBUG_LBL, stmt->as.lable);
            break;
        case STATEMENT_INSTRUCTION_T:
            ir_dump_instruction(&stmt->as.inst);
            break;
        case STATEMENT_DIRECTIVE_T:
            ir_dump_directive(&stmt->as.dir);
            break;
        case STATEMENT_DECLARATION_T:
            ir_dump_declaration(&stmt->as.decl);
            break;
        case STATEMENT_BLOCK_T:
            ir_dump_block(&stmt->as.blk);
            break;
    }
}

void ir_dump_ast(struct AST* ast) {
    DEBUG(IR_DEBUG_AST_DES, {
        for (int i = 0; i < ast->count; i++) {
            ir_dump_statement(&ast->statements[i]);
        }
        DEBUG_EN(IR_DEBUG_AST_END);
    }, ast->count);
}
void ir_dump_lexer(FILE* source) {
    DEBUG(LEXER_DEBUG_DES, {
        struct LEXEME_TOKEN token;
        int current_line = -1;
        while (lexer(source, &token, false)) {
            unsigned int lno = 0;
            if (token.type == LEXEME_PUN) lno = token.as.pun.line_no;
            else if (token.type == LEXEME_OPR) lno = token.as.opr.line_no;
            else if (token.type == LEXEME_LIT) lno = token.as.lit.line_no;
            else if (token.type == LEXEME_WRD) lno = token.as.wrd.line_no;
            
            if ((int)lno != current_line) {
                if (current_line != -1) { DBG("\n"); }
                current_line = lno;
                { DBG(LEXER_DEBUG_LNO, current_line); }
            }
            
            if (token.type == LEXEME_PUN) { DBG(LEXER_DEBUG_PUN, token.as.pun.data); }
            else if (token.type == LEXEME_OPR) { DBG(LEXER_DEBUG_OPR, token.as.opr.data); }
            else if (token.type == LEXEME_LIT) { DBG(LEXER_DEBUG_LIT, token.as.lit.data); }
            else if (token.type == LEXEME_WRD) { DBG(LEXER_DEBUG_WRD, token.as.wrd.data); }
        }
        if (current_line != -1) { DBG("\n"); }
        DEBUG_EN(LEXER_DEBUG_END);
    }, "");
}
