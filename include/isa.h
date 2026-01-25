#ifndef ASSEMBLER_ISA_API_H
#define ASSEMBLER_ISA_API_H

typedef struct ASSEMBLER_ISA{
    const char* name;
    const char* desc;
    void (*instruction)(void);
} AssemblerISA;

extern const struct ASSEMBLER_ISA* supported_isa_array[32];
extern unsigned int supported_isa_count;

void register_isa(const struct ASSEMBLER_ISA* isa);

#endif
