#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

#include "ir.h"
#include "pch.h"

// SYMBOL_TYPE
// Defines the type of symbol we are storing in the Symbol Table.
typedef enum SYMBOL_TYPE {
    SYMBOL_LABLE_T,
    SYMBOL_CONSTANT_T,
    SYMBOL_DATA_T
} SymbolType, SymType;

// SYMBOL_LABLE
// Represents a label mapped to a specific memory address
typedef struct SYMBOL_LABLE {
    unsigned long address;
} SymbolLable, SymLable;

// SYMBOL_CONSTANT
// Represents a constant mapped to an evaluated value
typedef struct SYMBOL_CONSTANT {
    unsigned long value;
    char modifier;
} SymbolConstant, SymConst;

// SYMBOL_DATA
// Represents an initialized or reserved memory location
typedef struct SYMBOL_DATA {
    unsigned long address;
    unsigned long size;
    char modifier;
} SymbolData, SymData;

// SYMBOL
// A linked-list node representing a symbol in the hash table
typedef struct SYMBOL {
    char* name;
    enum SYMBOL_TYPE type;
    
    union {
        struct SYMBOL_LABLE    lable;
        struct SYMBOL_CONSTANT constant;
        struct SYMBOL_DATA     data;
    } as;
    
    struct SYMBOL* next; // Separate chaining for hash collisions
} Symbol, Sym;

// SYMBOL_TABLE
// The core hash map mapping string identifiers to Symbols
typedef struct SYMBOL_TABLE {
    struct SYMBOL** entries; // Array of linked lists
    unsigned int capacity;   // Number of buckets
    unsigned int count;      // Total number of symbols stored
} SymbolTable, SymTab;

// Operations
struct SYMBOL_TABLE* symbol_table_create(unsigned int capacity);
void symbol_table_destroy(struct SYMBOL_TABLE* table);
bool symbol_table_insert(struct SYMBOL_TABLE* table, struct SYMBOL* symbol);
struct SYMBOL* symbol_table_lookup(struct SYMBOL_TABLE* table, const char* name);

// Traversal and Population
struct AST;
void ir_populate_symbol_table(struct AST* ast, struct SYMBOL_TABLE* symtab);

// Debug Formatting
void ir_dump_symbol_table(struct SYMBOL_TABLE* table);

#endif
