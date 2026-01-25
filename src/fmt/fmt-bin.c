#include "fmt.h"
#include "pch.h"

void bin_format();

static const struct ASSEMBLER_FMT formatter = {
    .name = "bin",
    .desc = "format that ommits raw binary file",
    .format = bin_format,
};

void bin_format(){
    printf("Helo BIN\n");
}

__attribute__((constructor)) static void register_bin(){
    register_fmt(&formatter);
}
