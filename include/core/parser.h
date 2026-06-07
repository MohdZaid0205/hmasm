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
    
    // ARITHMATIC and BINARY OPERATIONS
    INSTRUCTION_ADD,    // intiger addition
    INSTRUCTION_SUB,    // intiger substraction
    INSTRUCTION_MUL,    // intiger multiplication
    INSTRUCTION_DIV,    // intiger divison
    INSTRUCTION_INC,    // increment by 1
    INSTRUCTION_DEC,    // decrement by 1

    // BITWISE OPERATIONS
    INSTRUCTION_AND,    // bitwise and
    INSTRUCTION_OR,     // bitwise or
    INSTRUCTION_XOR,    // bitwise xor
    INSTRUCTION_NOT,    // bitwise not

    // SHIFTING OPERATIONS
    INSTRUCTION_SHR,    // shift right
    INSTRUCTION_SHL,    // shift left

    // BRANCHING and JUMPS
    INSTRUCTION_BEQ,    // condition: equal
    INSTRUCTION_BNEQ,   // condition: not equal
    INSTRUCTION_BG,     // condidion: greater
    INSTRUCTION_BGEQ,   // condition: greter or equal
    INSTRUCTION_BS,     // condition: smaller
    INSTRUCTION_BSEQ,   // condition: smaller or equal
    INSTRUCTION_JMP,    // jumps unconditionally
    INSTRUCTION_CALL,   // call to specific lable
    INSTRUCTION_RET,    // return to last caller

    // MISCELLANEOUS
    INSTRUCTION_NOP,    // no operation
    INSTRUCTION_HLT,    // halt execution
    INSTRUCTION_INT,    // interrupt
    INSTRUCTION_STI,    // stop interrupts
    INSTRUCTION_CLI,    // clear interrupts
} InstructionType, InstType;

typedef enum INSTRUCTION_COMPONENT_TYPE {
    INSTRUCTION_COMPONENT_REGISTER_T,
    INSTRUCTION_COMPONENT_ADDRESS_T,
    INSTRUCTION_COMPONENT_IMMIDIATE_T,
} InstructionComponentType, InstComponentType;


typedef struct INSTRUCTION_COMPONENT {
    enum INSTRUCTION_COMPONENT_TYPE type;
    union {
        // struct INSTRUCTION_COMPONENT_REGISTER    reg;
        // struct INSTRUCTION_COMPONENT_ADDRESS     adr;
        // struct INSTRUCTION_COMPONENT_IMMIDIATE   imm;
    } as;
} InstructionComponent, InstComponent;


#endif
