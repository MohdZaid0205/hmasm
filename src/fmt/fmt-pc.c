#include "fmt.h"
#include "pch.h"

void pc_format();

static const struct ASSEMBLER_FMT formatter = {
    .name = "pc",
    .desc = "windows executable format",
    .format = pc_format,
};

void pc_format(){
    printf("Helo PC\n");
}

__attribute__((constructor)) static void register_pc(){
    register_fmt(&formatter);
}
