#include "registery.h"
#include <string.h>

#define MAX_HANDLERS 256

typedef struct {
    const char* arch;
    const char* mnemonic;
    InstructionHandler handler;
} RegisteredInstruction;

static RegisteredInstruction instruction_registery[MAX_HANDLERS];
static unsigned int instruction_count = 0;

void register_instruction(const char* arch, const char* mnemonic, InstructionHandler handler) {
    if (instruction_count < MAX_HANDLERS) {
        instruction_registery[instruction_count].arch = arch;
        instruction_registery[instruction_count].mnemonic = mnemonic;
        instruction_registery[instruction_count].handler = handler;
        instruction_count++;
    }
}

InstructionHandler lookup_instruction(const char* arch, const char* mnemonic) {
    for (unsigned int i = 0; i < instruction_count; i++) {
        if (strcmp(instruction_registery[i].arch, arch) == 0 &&
            strcmp(instruction_registery[i].mnemonic, mnemonic) == 0) {
            return instruction_registery[i].handler;
        }
    }
    return 0;
}
