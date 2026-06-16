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
