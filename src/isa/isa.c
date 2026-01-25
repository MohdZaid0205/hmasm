#include "isa.h"

const struct ASSEMBLER_ISA* supported_isa_array[32];
unsigned int supported_isa_count = 0;

void register_isa(const struct ASSEMBLER_ISA* isa){
    if (supported_isa_count < 32){
        supported_isa_array[supported_isa_count] = isa;
        supported_isa_count++;
    }
}

