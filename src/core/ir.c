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
