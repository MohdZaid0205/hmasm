#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "pch.h"
#include "ir.h"
#include "lexer.h"

// AST aka AbstractSyntaxTree
// AST holds an array of parsed statements that represent the fully constructed
// Abstract Syntax Tree of the assembly source file.
typedef struct AST {
    struct STATEMENT* statements;       // dynamic array of statements
    int count;                          // number of valid statements
    int capacity;                       // allocated capacity of array
} Ast;

// parse
// Main entry point for the parser. It continually pulls tokens from the lexer
// and constructs the AST until EOF is reached.
// Returns true on success, false if a syntax error occurred.
struct ASSEMBLER_ISA;
bool parse(FILE* source, struct AST* out, const struct ASSEMBLER_ISA* isa);

#endif
