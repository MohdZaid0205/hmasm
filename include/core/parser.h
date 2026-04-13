#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "pch.h"

// [ INFO ] --------------------------------------------------------------------+
// parser for assembler directly operates on lexemes provided by lexer as out   |
// these lexemes are then processed sequentially by the parser in order to deal |
// with language specific requirement that are to be implemented by assembly    |
//                                                                              |
// * INSTRUCTIONS: an executable line of code that can be executed.             |
// * REGISTERS   : set of recognized registers as per isa.                      |
// * IMMIDEATE   : direct representation of numeric values.                     |
// * ADDRESS     : direct/indirect representation of memmory.                   |
// -----------------------------------------------------------------------------+


// this ia a top level idea for future reference
typedef struct ASSEMBLER_INSTRUCTION {
    enum ASSEMBLER_INSTRUCTION_TYPE type;
    struct {
        enum ASSEMBLER_COMPONENT_TYPE type;
        union {
            struct ASSEMBLER_REGISTER  reg;
            struct ASSEMBLER_ADDRESS   add;
            struct ASSEMBLER_IMMIDEATE imm;
        } content;
    }  des;  // destination
    struct {
        enum ASSEMBLER_COMPONENT_TYPE type;
        union {
            struct ASSEMBLER_REGISTER  reg;
            struct ASSEMBLER_ADDRESS   add;
            struct ASSEMBLER_IMMIDEATE imm;
        } content;
    }* src;  // set of sources
} AssemblerInstruction, Instruction;

#endif
