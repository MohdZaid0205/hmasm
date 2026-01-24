#ifndef ASSEMBLER_FMT_API_H
#define ASSEMBLER_FMT_API_H

typedef struct ASSEMBLER_FMT{
    const char* name;
    const char* desc;
    void (*format)(void);
} AssemblerFMT;

extern const struct ASSEMBLER_FMT* supported_fmt_array[32];
extern unsigned int supported_fmt_count;

void register_fmt(const struct ASSEMBLER_FMT* fmt);

#endif
