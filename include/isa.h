#ifndef ASSEMBLER_ISA_API_H
#define ASSEMBLER_ISA_API_H

#include <stdbool.h>
#include <stdlib.h>
#include "ir.h"
#include "exceptions.h"
#include "strdump.h"
#include "registery.h"

typedef struct ASSEMBLER_ISA {
    const char* name;
    const char* desc;
    bool (*is_register)(const char* token);
    unsigned int (*validate_and_get_size)(struct INSTRUCTION* inst);
    unsigned int (*encode_instruction)(struct INSTRUCTION* inst, unsigned char* out_buffer, unsigned long current_address);
    unsigned int (*encode_raw)(const char* raw_asm, unsigned char* out_buffer, unsigned long current_address);
} AssemblerISA;

extern const struct ASSEMBLER_ISA* supported_isa_array[32];
extern unsigned int supported_isa_count;

void register_isa(const struct ASSEMBLER_ISA* isa);

#define IMPLEMENT_INSTRUCTION(arch, mnemonic) \
    void arch##_instruction_##mnemonic(struct INSTRUCTION* inst, unsigned char* out_buffer, unsigned long current_address); \
    __attribute__((constructor)) static void arch##_register_##mnemonic(void) { \
        register_instruction(#arch, #mnemonic, arch##_instruction_##mnemonic); \
    } \
    void arch##_instruction_##mnemonic(struct INSTRUCTION* inst, unsigned char* out_buffer, unsigned long current_address)

#define SIGNATURE(t1, t2, t3) \
    if (((int)inst->rd.type == INSTRUCTION_COMPONENT_##t1##_T) && \
        ((int)inst->rs1.type == INSTRUCTION_COMPONENT_##t2##_T) && \
        ((int)inst->rs2.type == INSTRUCTION_COMPONENT_##t3##_T))

#define INSTRUCTION_COMPONENT_NONE_T -1

#define SIGNATURE_MISS_MATCH_EXCEPTION(inst_name, body)                         \
    EXCEPTION(ISA_ISM_DES, {                                                    \
        EXCEPTION_LN(INSET, ISA_ISM_LN1);                                       \
        INFORMATION_LN(INSET, ISA_ISM_LN2);                                     \
        { body }                                                                \
        EXCEPTION_EN(ISA_ISM_END, #inst_name);                                  \
    }, #inst_name)

#define OTHERWISE(inst_name, body) \
    else { SIGNATURE_MISS_MATCH_EXCEPTION(inst_name, body); exit(1); }

#endif
