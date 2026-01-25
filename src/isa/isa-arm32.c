#include "isa.h"
#include "pch.h"

void arm32_instruction();

static const struct ASSEMBLER_ISA architecture = {
    .name = "arm32",
    .desc = "arm based 32 bit",
    .instruction = arm32_instruction,
};

void arm32_instruction(){
    printf("Hello arm32\n");
}

__attribute__((constructor)) static void register_arm32(){
    register_isa(&architecture);
}
