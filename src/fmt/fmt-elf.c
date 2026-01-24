#include "fmt.h"
#include "pch.h"

void elf_format();

static const struct ASSEMBLER_FMT formatter = {
    .name = "elf",
    .desc = "unix executable and linkable format",
    .format = elf_format,
};

void elf_format(){
    printf("Helo ELF\n");
}

__attribute__((constructor)) static void register_elf(){
    register_fmt(&formatter);
}
