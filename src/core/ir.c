#include "ir.h"
#include "parser.h"
#include "exceptions.h"
#include "logging.h"
#include "strdump.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ir_dump_block(struct BLOCK* blk) {
    if (blk->type == BLOCK_OPTIMIZATION_T) {
        DEBUG_LN("", IR_DEBUG_BLK_OPT, blk->as.opt.arch ? blk->as.opt.arch : "UNKNOWN");
    } else if (blk->type == BLOCK_MACRO_T) {
        DEBUG_LN("", IR_DEBUG_BLK_MAC, blk->as.mac.name ? blk->as.mac.name : "UNKNOWN");
    }
    DEBUG_LN("", IR_DEBUG_BLK_RAW);
    
    // Print each line of raw data
    char* data = strdup(blk->data);
    char* line = strtok(data, "\n");
    while (line) {
        DEBUG_LN("", IR_DEBUG_BLK_RLN, line);
        line = strtok(NULL, "\n");
    }
    free(data);
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
