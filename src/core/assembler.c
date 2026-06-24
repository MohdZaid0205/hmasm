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


struct ALIAS {
    char* name;
    char* reg;
};
static struct ALIAS aliases[128];
static int alias_count = 0;

static void process_ast(struct AST* ast, const struct ASSEMBLER_ISA* isa, struct ASSEMBLER_SEGMENT** p_current_seg, struct ASSEMBLER_SEGMENT* text_seg, struct ASSEMBLER_SEGMENT* data_seg, struct ASSEMBLER_SEGMENT* rodata_seg, struct ASSEMBLER_SEGMENT* bss_seg) {
    struct ASSEMBLER_SEGMENT* current_seg = *p_current_seg;
    for (unsigned int i = 0; i < ast->count; i++) {
        struct STATEMENT* stmt = &ast->statements[i];
        
        if (stmt->type == STATEMENT_DIRECTIVE_T && stmt->as.dir.type == DIRECTIVE_SECTION_T) {
            if (strcmp(stmt->as.dir.target, "text") == 0) {
                current_seg = text_seg;
            } else if (strcmp(stmt->as.dir.target, "data") == 0) {
                current_seg = data_seg;
            } else if (strcmp(stmt->as.dir.target, "rodata") == 0) {
                current_seg = rodata_seg;
            } else if (strcmp(stmt->as.dir.target, "bss") == 0) {
                current_seg = bss_seg;
            }
        } else if (stmt->type == STATEMENT_DIRECTIVE_T && stmt->as.dir.type == DIRECTIVE_REGISTER_ALIAS_T) {
            if (stmt->as.dir.arch && (strcmp(stmt->as.dir.arch, isa->name) == 0 || strcmp(stmt->as.dir.arch, "default") == 0)) {
                if (alias_count < 128) {
                    aliases[alias_count].name = strdup(stmt->as.dir.target);
                    aliases[alias_count].reg = strdup(stmt->as.dir.modifier);
                    alias_count++;
                }
            }
        } else if (stmt->type == STATEMENT_BLOCK_T && stmt->as.blk.type == BLOCK_OPTIMIZATION_T) {
            char* target_chunk = NULL;
            char* default_chunk = NULL;
            
            for (int i = 0; i < stmt->as.blk.as.opt.chunk_count; i++) {
                if (strcasecmp(stmt->as.blk.as.opt.chunks[i].arch, isa->name) == 0) {
                    target_chunk = strdup(stmt->as.blk.as.opt.chunks[i].data);
                } else if (strcasecmp(stmt->as.blk.as.opt.chunks[i].arch, "default") == 0) {
                    default_chunk = strdup(stmt->as.blk.as.opt.chunks[i].data);
                }
            }
            
            if (target_chunk) {
                for (int k = 0; k < alias_count; k++) {
                    char* p = target_chunk;
                    int len_name = strlen(aliases[k].name);
                    int len_reg = strlen(aliases[k].reg);
                    while ((p = strstr(p, aliases[k].name)) != NULL) {
                        // check word boundaries
                        int valid = 1;
                        if (p > target_chunk) {
                            char c = *(p-1);
                            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') valid = 0;
                        }
                        char n = *(p + len_name);
                        if ((n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z') || (n >= '0' && n <= '9') || n == '_') valid = 0;
                        
                        if (valid) {
                            if (len_reg <= len_name) {
                                memcpy(p, aliases[k].reg, len_reg);
                                memmove(p + len_reg, p + len_name, strlen(p + len_name) + 1);
                                p += len_reg;
                            } else {
                                char* new_chunk = malloc(strlen(target_chunk) + len_reg - len_name + 1);
                                int prefix_len = p - target_chunk;
                                memcpy(new_chunk, target_chunk, prefix_len);
                                memcpy(new_chunk + prefix_len, aliases[k].reg, len_reg);
                                strcpy(new_chunk + prefix_len + len_reg, p + len_name);
                                free(target_chunk);
                                target_chunk = new_chunk;
                                p = target_chunk + prefix_len + len_reg;
                            }
                        } else {
                            p += len_name;
                        }
                    }
                }

                int success = 0;
                if (isa->encode_raw) {
                    unsigned int encoded_size = isa->encode_raw(target_chunk, current_seg->buffer + current_seg->size, current_seg->base_address + current_seg->size);
                    if (encoded_size > 0) {
                        current_seg->size += encoded_size;
                        success = 1;
                    }
                }
                free(target_chunk);
                
                if (!success && default_chunk) {
                    INF("%([ BLOCK: OPTIMIZATION ]%) Native compilation %(FAILED%), falling back to %(@default%) chunk..." NLINE);
                    FILE* mem_stream = fmemopen(default_chunk, strlen(default_chunk), "r");
                    if (mem_stream) {
                        struct AST sub_ast;
                        if (parse(mem_stream, &sub_ast, isa)) {
                            process_ast(&sub_ast, isa, &current_seg, text_seg, data_seg, rodata_seg, bss_seg);
                        }
                        fclose(mem_stream);
                    }
                }
                if (default_chunk) free(default_chunk);
            } else if (default_chunk) {
                INF("%([ BLOCK: OPTIMIZATION ]%) No native chunk found, using %(@default%) chunk..." NLINE);
                FILE* mem_stream = fmemopen(default_chunk, strlen(default_chunk), "r");
                if (mem_stream) {
                    struct AST sub_ast;
                    if (parse(mem_stream, &sub_ast, isa)) {
                        process_ast(&sub_ast, isa, &current_seg, text_seg, data_seg, rodata_seg, bss_seg);
                    }
                    fclose(mem_stream);
                }
                free(default_chunk);
            }
            
        } else if (stmt->type == STATEMENT_INSTRUCTION_T && isa->encode_instruction) {
            // Apply register aliases
            struct INSTRUCTION_COMPONENT* comps[3] = {&stmt->as.inst.rd, &stmt->as.inst.rs1, &stmt->as.inst.rs2};
            for (int c = 0; c < 3; c++) {
                struct INSTRUCTION_COMPONENT* comp = comps[c];
                if (comp->type == INSTRUCTION_COMPONENT_REGISTER_T || comp->type == INSTRUCTION_COMPONENT_IMMIDIATE_T) {
                    for (int k = 0; k < alias_count; k++) {
                        const char* name = (comp->type == INSTRUCTION_COMPONENT_REGISTER_T) ? comp->as.reg.name : comp->as.imm.lable;
                        if (name && strcmp(name, aliases[k].name) == 0) {
                            if (comp->type == INSTRUCTION_COMPONENT_IMMIDIATE_T) {
                                free(comp->as.imm.lable);
                                comp->type = INSTRUCTION_COMPONENT_REGISTER_T;
                            } else {
                                free(comp->as.reg.name);
                            }
                            comp->as.reg.name = strdup(aliases[k].reg);
                            break;
                        }
                    }
                }
            }
            
            unsigned int encoded_size = isa->encode_instruction(
                &stmt->as.inst, 
                current_seg->buffer + current_seg->size, 
                current_seg->base_address + current_seg->size
            );
            current_seg->size += encoded_size;
        } else if (stmt->type == STATEMENT_DECLARATION_T) {
            if (stmt->as.decl.type == DECLARATION_COMPONENT_DATA_T && stmt->as.decl.as.vals.count > 0) {
                const char* lit = stmt->as.decl.as.vals.values[0].value;
                int len = strlen(lit);
                if (len >= 2 && lit[0] == '"' && lit[len-1] == '"') {
                    for (int j = 1; j < len - 1; j++) {
                        if (lit[j] == '\\' && j + 1 < len - 1 && lit[j+1] == 'n') {
                            current_seg->buffer[current_seg->size++] = '\n';
                            j++;
                        } else {
                            current_seg->buffer[current_seg->size++] = lit[j];
                        }
                    }
                    current_seg->buffer[current_seg->size++] = 0;
                }
            } else if (stmt->as.decl.type == DECLARATION_COMPONENT_RESERVE_T) {
                current_seg->size += stmt->as.decl.as.resv.size;
            }
        }
    }
    *p_current_seg = current_seg;
}

int main(int argc, char** argv) {    
    argparse(argc, argv);
    
    if (_argparse_req_help) return 0;
    
    if (!_argparse_source_file) {
        EXCEPTION("No source file provided for assembly.", {
            EXCEPTION_LN("\t", "Usage: hmasm [options] <source_file>" NLINE);
            EXCEPTION_EN("missing arguments" NLINE);
        }, "");
        return 1;
    }
    
    FILE* src = fopen(_argparse_source_file, "r");
    if (!src) {
        EXCEPTION("Could not open source file %s\n", {
            EXCEPTION_LN("\t", "Please verify that the file exists and is accessible." NLINE);
            EXCEPTION_EN("file operation failed" NLINE);
        }, _argparse_source_file);
        return 1;
    }
    
    // Dump Lexer if debug is enabled
    ir_dump_lexer(src);
    fseek(src, 0, SEEK_SET);
    
    struct AST ast;
    if (!parse(src, &ast, &riscv_isa)) {
        EXCEPTION("The parser encountered a fatal error and could not generate an AST." NLINE, {
            EXCEPTION_LN("\t", "Review the syntax of the input source file." NLINE);
            EXCEPTION_EN("parser failed" NLINE);
        }, "");
        fclose(src);
        return 1;
    }
    
    // Dump AST if debug is enabled
    ir_dump_ast(&ast);
    
    // Allocate segment buffers dynamically using a predefined structure
    struct ASSEMBLER_SEGMENT text_seg = { .name = "text", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT data_seg = { .name = "data", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT rodata_seg = { .name = "rodata", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT bss_seg = { .name = "bss", .buffer = malloc(4096), .size = 0, .base_address = 0 };
    struct ASSEMBLER_SEGMENT* current_seg = &text_seg;
    
        const struct ASSEMBLER_ISA* target_isa = NULL;
    for (int i = 0; i < supported_isa_count; i++) {
        if (strcmp(_argparse_isa_type, supported_isa_array[i]->name) == 0) {
            target_isa = supported_isa_array[i];
            break;
        }
    }
    
    if (!target_isa) {
        // Fallback or error
        target_isa = supported_isa_array[0]; // Or return error
    }

    process_ast(&ast, target_isa, &current_seg, &text_seg, &data_seg, &rodata_seg, &bss_seg);
    
    struct ASSEMBLER_SEGMENT* segments[] = { &text_seg, &rodata_seg, &data_seg, &bss_seg };
    
    if (_argparse_output_file) {
        if (bin_fmt.emit_file(_argparse_output_file, NULL, segments, 4)) {
            INF("Successfully emitted output to %(%s%)" NLINE, _argparse_output_file);
        } else {
            EXCEPTION("Failed to emit binary output to %s" NLINE, {
                EXCEPTION_LN("\t", "Ensure you have write permissions to the destination path." NLINE);
                EXCEPTION_EN("format generation failed" NLINE);
            }, _argparse_output_file);
        }
    }
    
    free(text_seg.buffer);
    free(data_seg.buffer);
    free(rodata_seg.buffer);
    free(bss_seg.buffer);
    fclose(src);
    
    return 0;
}
