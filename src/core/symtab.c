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
