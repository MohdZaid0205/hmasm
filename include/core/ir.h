#ifndef ASSEMBELR_IR_H
#define ASSEMBELR_IR_H

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

typedef struct INSTRUCTION_COMPONENT_REGISTER {
    char* name;         // name of required register
    int   size;         // to check for size mismatches
} InstructionComponentRegister, InstRegister, Reg;

typedef struct INSTRUCTION_COMPONENT_ADDRESS {
    char* name;         // name of register with address
    char* offset;       // provided offset to address
} InstructionComponentAddress, InstAddress, Adr;

typedef struct INSTRUCTION_COMPONENT_IMMIDIATE {
    char* value;        // if direct value was provided
    char* lable;        // if a lable was provided
} InstructionComponentImmidiate, InstImmidiate, Imm;

typedef struct INSTRUCTION_COMPONENT {
    enum INSTRUCTION_COMPONENT_TYPE type;
    union {
        struct INSTRUCTION_COMPONENT_REGISTER    reg;
        struct INSTRUCTION_COMPONENT_ADDRESS     adr;
        struct INSTRUCTION_COMPONENT_IMMIDIATE   imm;
    } as;
} InstructionComponent, InstComponent;

typedef struct INSTRUCTION {
    char* name;
    struct INSTRUCTION_COMPONENT rd;
    struct INSTRUCTION_COMPONENT rs1;
    struct INSTRUCTION_COMPONENT rs2;
} Instruction;


#endif
