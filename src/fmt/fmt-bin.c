#include "fmt.h"
#include <stdio.h>

IMPLEMENT_FORMATTER(bin) {
    FILE* out = fopen(output_path, "wb");
    if (!out) return false;
    
    SECTION("text") {
        if (segment->buffer && segment->size > 0) {
            fwrite(segment->buffer, 1, segment->size, out);
        }
    }

    SECTION("rodata") {
        if (segment->buffer && segment->size > 0) {
            fwrite(segment->buffer, 1, segment->size, out);
        }
    }

    SECTION("data") {
        if (segment->buffer && segment->size > 0) {
            fwrite(segment->buffer, 1, segment->size, out);
        }
    }
    
    SECTION("bss") {
        if (segment->size > 0) {
            // raw bin fills bss with zeroes
            for (unsigned int i = 0; i < segment->size; i++) {
                fputc(0, out);
            }
        }
    }
    
    fclose(out);
    return true;
}

const struct ASSEMBLER_FMT bin_fmt = {
    .name = "bin",
    .extension = ".bin",
    .desc = "Raw Binary Format",
    .emit_file = fmt_bin_emit
};
