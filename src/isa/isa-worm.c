#include "isa.h"
#include "pch.h"

void riscv32_instruction();

static const struct ASSEMBLER_ISA architecture = {
    .name = "worm",
    .desc = "Worm Instruction Set (for simple virtual machine)",
    .instruction = riscv32_instruction,
};

void riscv32_instruction(){
    printf("Hello Worm!\n");
}

__attribute__((constructor)) static void register_riscv32(){
    register_isa(&architecture);
}
