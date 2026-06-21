#include "symtab.h"
#include "exceptions.h"
#include "logging.h"
#include "strdump.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// FNV-1a 32-bit hash function
static unsigned int __hash_fnv1a(const char* key) {
    unsigned int hash = 2166136261u;
    while (*key) {
        hash ^= (unsigned char)(*key);
        hash *= 16777619u;
        key++;
    }
    return hash;
}

struct SYMBOL_TABLE* symbol_table_create(unsigned int capacity) {
    struct SYMBOL_TABLE* table = malloc(sizeof(struct SYMBOL_TABLE));
    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(capacity, sizeof(struct SYMBOL*));
    return table;
}

void symbol_table_destroy(struct SYMBOL_TABLE* table) {
    if (!table) return;
    for (unsigned int i = 0; i < table->capacity; i++) {
        struct SYMBOL* current = table->entries[i];
        while (current) {
            struct SYMBOL* next = current->next;
            if (current->name) free(current->name);
            free(current);
            current = next;
        }
    }
    free(table->entries);
    free(table);
}

bool symbol_table_insert(struct SYMBOL_TABLE* table, struct SYMBOL* symbol) {
    if (!table || !symbol || !symbol->name) return false;
    
    // Check if it already exists
    if (symbol_table_lookup(table, symbol->name) != NULL) {
        return false; // Duplicate symbol
    }
    
    unsigned int index = __hash_fnv1a(symbol->name) % table->capacity;
    
    // Copy the symbol into the table
    struct SYMBOL* new_sym = malloc(sizeof(struct SYMBOL));
    *new_sym = *symbol;
    new_sym->name = strdup(symbol->name);
    
    // Insert at head of the chain
    new_sym->next = table->entries[index];
    table->entries[index] = new_sym;
    table->count++;
    
    return true;
}

struct SYMBOL* symbol_table_lookup(struct SYMBOL_TABLE* table, const char* name) {
    if (!table || !name) return NULL;
    
    unsigned int index = __hash_fnv1a(name) % table->capacity;
    struct SYMBOL* current = table->entries[index];
    
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

static unsigned long __get_datatype_size(enum DECLARATION_DATA_TYPE dt) {
    switch (dt) {
        case DECLARATION_B_T: return 1;
        case DECLARATION_W_T: return 2;
        case DECLARATION_D_T: return 4;
        case DECLARATION_Q_T: return 8;
        case DECLARATION_T_T: return 10;
        case DECLARATION_Y_T: return 16;
        case DECLARATION_Z_T: return 32;
        default: return 1;
    }
}

static char __get_datatype_modifier(enum DECLARATION_DATA_TYPE dt) {
    switch (dt) {
        case DECLARATION_B_T: return 'B';
        case DECLARATION_W_T: return 'W';
        case DECLARATION_D_T: return 'D';
        case DECLARATION_Q_T: return 'Q';
        case DECLARATION_T_T: return 'T';
        case DECLARATION_Y_T: return 'Y';
        case DECLARATION_Z_T: return 'Z';
        default: return 'B';
    }
}

void ir_populate_symbol_table(struct AST* ast, struct SYMBOL_TABLE* symtab) {
    if (!ast || !symtab) return;
    
    unsigned long current_address = 0x01000000;
    
    for (int i = 0; i < ast->count; i++) {
        struct STATEMENT* stmt = &ast->statements[i];
        
        switch (stmt->type) {
            case STATEMENT_LABEL_T: {
                struct SYMBOL sym;
                sym.name = stmt->as.lable;
                sym.type = SYMBOL_LABLE_T;
                sym.as.lable.address = current_address;
                symbol_table_insert(symtab, &sym);
                break;
            }
            case STATEMENT_DECLARATION_T: {
                struct DECLARATION* decl = &stmt->as.decl;
                struct SYMBOL sym;
                sym.name = decl->name;
                
                if (decl->type == DECLARATION_COMPONENT_CONST_T) {
                    sym.type = SYMBOL_CONSTANT_T;
                    sym.as.constant.modifier = __get_datatype_modifier(decl->dt);
                    
                    // Simple parsing of immediate value literal for mock (assuming hex/dec)
                    sym.as.constant.value = strtol(decl->as.cnst.value.value, NULL, 0);
                    symbol_table_insert(symtab, &sym);
                    
                } else if (decl->type == DECLARATION_COMPONENT_DATA_T) {
                    sym.type = SYMBOL_DATA_T;
                    sym.as.data.modifier = __get_datatype_modifier(decl->dt);
                    sym.as.data.address = current_address;
                    sym.as.data.size = decl->as.vals.count;
                    symbol_table_insert(symtab, &sym);
                    
                    current_address += __get_datatype_size(decl->dt) * decl->as.vals.count;
                    
                } else if (decl->type == DECLARATION_COMPONENT_RESERVE_T) {
                    sym.type = SYMBOL_DATA_T;
                    sym.as.data.modifier = __get_datatype_modifier(decl->dt);
                    sym.as.data.address = current_address;
                    sym.as.data.size = decl->as.resv.size;
                    symbol_table_insert(symtab, &sym);
                    
                    current_address += __get_datatype_size(decl->dt) * decl->as.resv.size;
                }
                break;
            }
            case STATEMENT_INSTRUCTION_T: {
                // Mock instruction size (fixed 4 bytes for now)
                current_address += 4;
                break;
            }
            default:
                break;
        }
    }
}

void ir_dump_symbol_table(struct SYMBOL_TABLE* table) {
    DEBUG(IR_DEBUG_SYM_DES, {
        for (unsigned int i = 0; i < table->capacity; i++) {
            struct SYMBOL* current = table->entries[i];
            while (current) {
                switch (current->type) {
                    case SYMBOL_LABLE_T:
                        DEBUG_LN("", IR_DEBUG_SYM_LBL, current->name, current->as.lable.address);
                        break;
                    case SYMBOL_CONSTANT_T:
                        DEBUG_LN("", IR_DEBUG_SYM_CON, current->name, current->as.constant.modifier, current->as.constant.value);
                        break;
                    case SYMBOL_DATA_T:
                        DEBUG_LN("", IR_DEBUG_SYM_DAT, current->name, current->as.data.modifier, current->as.data.address, current->as.data.size);
                        break;
                }
                current = current->next;
            }
        }
        DEBUG_EN(IR_DEBUG_SYM_END);
    }, table->count);
}
