#ifndef HMASM_REGISTERY_H
#define HMASM_REGISTERY_H

struct INSTRUCTION;

typedef void (*InstructionHandler)(struct INSTRUCTION* inst, unsigned char* out_buffer, unsigned long current_address);

void register_instruction(const char* arch, const char* mnemonic, InstructionHandler handler);
InstructionHandler lookup_instruction(const char* arch, const char* mnemonic);

#endif
