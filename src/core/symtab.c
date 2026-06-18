#include "symtab.h"
#include "exceptions.h"
#include "logging.h"
#include "strdump.h"
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
