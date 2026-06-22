#include "pch.h"
#include "exceptions.h"
#include "logging.h"
#include "argparse.h"
#include "lexer.h"
#include "isa.h"
#include "fmt.h"
#include "strdump.h"
#include "parser.h"
#include "ir.h"

extern const struct ASSEMBLER_ISA riscv_isa;
extern const struct ASSEMBLER_FMT bin_fmt;

int main(int argc, char** argv) {    
    argparse(argc, argv);
    
    if (_argparse_req_help) return 0;
    
    if (!_argparse_source_file) {
        printf("Error: No source file provided.\n");
        return 1;
    }
    
    FILE* src = fopen(_argparse_source_file, "r");
    if (!src) {
        printf("Error: Could not open source file %s\n", _argparse_source_file);
        return 1;
    }
    
    // Optionally dump Lexer if requested
    ir_dump_lexer(src);
    fseek(src, 0, SEEK_SET); // Rewind for the parser
    
    struct AST ast;
    if (!parse(src, &ast, &riscv_isa)) {
        printf("Parse failed!\n");
        return 1;
    }
    
    // Optionally dump AST if requested or always
    ir_dump_ast(&ast);
    
    // Allocate dummy segment buffers for bin compilation
    struct ASSEMBLER_SEGMENT text_seg = { .name = "text", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT data_seg = { .name = "data", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    
    struct ASSEMBLER_SEGMENT* current_seg = &text_seg;
    
    for (unsigned int i = 0; i < ast.count; i++) {
        struct STATEMENT* stmt = &ast.statements[i];
        if (stmt->type == STATEMENT_DIRECTIVE_T && stmt->as.dir.type == DIRECTIVE_SECTION_T) {
            if (strcmp(stmt->as.dir.target, "text") == 0) current_seg = &text_seg;
            else if (strcmp(stmt->as.dir.target, "data") == 0) current_seg = &data_seg;
        } else if (stmt->type == STATEMENT_INSTRUCTION_T) {
            if (riscv_isa.encode_instruction) {
                unsigned int size = riscv_isa.encode_instruction(&stmt->as.inst, current_seg->buffer + current_seg->size, current_seg->base_address + current_seg->size);
                current_seg->size += size;
            }
        }
    }
    
    struct ASSEMBLER_SEGMENT* segments[] = { &data_seg, &text_seg };
    
    if (_argparse_output_file) {
        if (bin_fmt.emit_file(_argparse_output_file, NULL, segments, 2)) {
            printf("Successfully emitted output to %s\n", _argparse_output_file);
        } else {
            printf("Failed to emit output to %s\n", _argparse_output_file);
        }
    }
    
    free(text_seg.buffer);
    free(data_seg.buffer);
    
    return 0;
}
