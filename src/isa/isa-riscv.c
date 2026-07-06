#include "isa.h"
#include "pch.h"

void riscv32_instruction();

static const struct ASSEMBLER_ISA architecture = {
    .name = "riscv",
    .desc = "Reduced Instruction Set Architecture 5",
    .instruction = riscv32_instruction,
};

void riscv32_instruction(){
    printf("Hello riscv!\n");
}

__attribute__((constructor)) static void register_riscv32(){
    register_isa(&architecture);
}
