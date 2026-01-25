#include "isa.h"
#include "pch.h"

void i386_instruction();

static const struct ASSEMBLER_ISA architecture = {
    .name = "i386",
    .desc = "intel 386 series 32 bit",
    .instruction = i386_instruction,
};

void i386_instruction(){
    printf("Hello i386\n");
}

__attribute__((constructor)) static void register_i386(){
    register_isa(&architecture);
}
