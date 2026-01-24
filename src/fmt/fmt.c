#include "fmt.h"

const struct ASSEMBLER_FMT* supported_fmt_array[32];
unsigned int supported_fmt_count = 0;

void register_fmt(const struct ASSEMBLER_FMT* fmt){
    if (supported_fmt_count < 32){
        supported_fmt_array[supported_fmt_count] = fmt;
        supported_fmt_count++;
    }
}

