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
        EXCEPTION("No source file provided for assembly.", {
            EXCEPTION_LN("\t", "Usage: hmasm [options] <source_file>");
            EXCEPTION_EN("missing arguments");
        }, "");
        return 1;
    }
    
    FILE* src = fopen(_argparse_source_file, "r");
    if (!src) {
        EXCEPTION("Could not open source file %s\n", {
            EXCEPTION_LN("\t", "Please verify that the file exists and is accessible.");
            EXCEPTION_EN("file operation failed");
        }, _argparse_source_file);
        return 1;
    }
    
    // Dump Lexer if debug is enabled
    ir_dump_lexer(src);
    fseek(src, 0, SEEK_SET);
    
    struct AST ast;
    if (!parse(src, &ast, &riscv_isa)) {
        EXCEPTION("The parser encountered a fatal error and could not generate an AST.\n", {
            EXCEPTION_LN("\t", "Review the syntax of the input source file.");
            EXCEPTION_EN("parser failed");
        }, "");
        fclose(src);
        return 1;
    }
    
    // Dump AST if debug is enabled
    ir_dump_ast(&ast);
    
    // Allocate segment buffers dynamically using a predefined structure
    struct ASSEMBLER_SEGMENT text_seg = { .name = "text", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT data_seg = { .name = "data", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT* current_seg = &text_seg;
    
    for (unsigned int i = 0; i < ast.count; i++) {
        struct STATEMENT* stmt = &ast.statements[i];
        
        if (stmt->type == STATEMENT_DIRECTIVE_T && stmt->as.dir.type == DIRECTIVE_SECTION_T) {
            if (strcmp(stmt->as.dir.target, "text") == 0) {
                current_seg = &text_seg;
            } else if (strcmp(stmt->as.dir.target, "data") == 0) {
                current_seg = &data_seg;
            }
        } else if (stmt->type == STATEMENT_INSTRUCTION_T && riscv_isa.encode_instruction) {
            unsigned int encoded_size = riscv_isa.encode_instruction(
                &stmt->as.inst, 
                current_seg->buffer + current_seg->size, 
                current_seg->base_address + current_seg->size
            );
            current_seg->size += encoded_size;
        }
    }
    
    struct ASSEMBLER_SEGMENT* segments[] = { &data_seg, &text_seg };
    
    if (_argparse_output_file) {
        if (bin_fmt.emit_file(_argparse_output_file, NULL, segments, 2)) {
            INF("Successfully emitted output to %(%s%)\n", _argparse_output_file);
        } else {
            EXCEPTION("Failed to emit binary output to %s\n", {
                EXCEPTION_LN("\t", "Ensure you have write permissions to the destination path.");
                EXCEPTION_EN("format generation failed");
            }, _argparse_output_file);
        }
    }
    
    free(text_seg.buffer);
    free(data_seg.buffer);
    fclose(src);
    
    return 0;
}
