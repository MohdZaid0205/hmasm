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


typedef enum INSTRUCTION_TYPE {
    INSTRUCTION_MOVE,   // move value from src to dest
    INSTRUCTION_PUSH,   // stack push
    INSTRUCTION_POP,    // stack pop
    
    // BINARY OPERATIONS
    INSTRUCTION_ADD,    // intiger addition
    INSTRUCTION_SUB,    // intiger substraction
    INSTRUCTION_MUL,    // intiger multiplication
    INSTRUCTION_DIV,    // intiger divison

    // BITWISE OPERATIONS
    INSTRUCTION_AND,    // bitwise and
    INSTRUCTION_OR,     // bitwise or
    INSTRUCTION_XOR,    // bitwise xor
    INSTRUCTION_NOT,    // bitwise not

    // SHIFTING OPERATIONS
    INSTRUCTION_SHR,    // shift right
    INSTRUCTION_SHL,    // shift left
    
    // BRANCHING and JUMPS
    INSTRUCTION_EQ,     // condition: equal
    INSTRUCTION_NEQ,    // condition: not equal
    INSTRUCTION_G,      // condidion: greater
    INSTRUCTION_GEQ,    // condition: greter or equal
    INSTRUCTION_S,      // condition: smaller
    INSTRUCTION_SEQ,    // condition: smaller or equal
    INSTRUCTION_JMP,    // jumps unconditionally
    INSTRUCTION_JIC,    // jump if previous condition
} InstructionType, InstType;




#endif
