#ifndef ASSEMBLER_FMT_API_H
#define ASSEMBLER_FMT_API_H

#include <stdbool.h>
#include <string.h>

struct SYMBOL_TABLE;

typedef struct ASSEMBLER_SEGMENT {
    char* name;
    unsigned char* buffer;
    unsigned int size;
    unsigned long base_address;
} AssemblerSegment;

typedef struct ASSEMBLER_FMT {
    const char* name;
    const char* extension;
    const char* desc;
    bool (*emit_file)(const char* output_path, struct SYMBOL_TABLE* symtab, struct ASSEMBLER_SEGMENT** segments, unsigned int segment_count);
} AssemblerFMT;

extern const struct ASSEMBLER_FMT* supported_fmt_array[32];
extern unsigned int supported_fmt_count;

void register_fmt(const struct ASSEMBLER_FMT* fmt);

#define IMPLEMENT_FORMATTER(fmt_name) \
    bool fmt_##fmt_name##_emit(const char* output_path, struct SYMBOL_TABLE* symtab, struct ASSEMBLER_SEGMENT** segments, unsigned int segment_count); \
    extern const struct ASSEMBLER_FMT fmt_name##_fmt; \
    __attribute__((constructor)) static void fmt_register_##fmt_name(void) { \
        register_fmt(&fmt_name##_fmt); \
    } \
    bool fmt_##fmt_name##_emit(const char* output_path, struct SYMBOL_TABLE* symtab, struct ASSEMBLER_SEGMENT** segments, unsigned int segment_count)

#define SECTION(section_name) \
    for (unsigned int __i = 0; __i < segment_count; __i++) \
        if (strcmp(segments[__i]->name, section_name) == 0) \
            for (struct ASSEMBLER_SEGMENT* segment = segments[__i]; segment != NULL; segment = NULL)

#endif
