#ifndef ASSEMBLER_REGISTERY_H
#define ASSEMBLER_REGISTERY_H

typedef struct ASSEMBLER_ISA {
	const char*  name;
	const char*  desc;
	void(*init)(void);
	;
} AssemblerISA;

#endif