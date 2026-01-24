#include "isa.h"
#include "pch.h"

void riscv32_instruction();

static const struct ASSEMBLER_ISA architecture = {
    .name = "riscv32",
    .desc = "reduced instruction set architecture 32 bit",
    .instruction = riscv32_instruction,
};

void riscv32_instruction(){
    printf("Hello riscv32\n");
}

__attribute__((constructor)) static void register_riscv32(){
    register_isa(&architecture);
}
