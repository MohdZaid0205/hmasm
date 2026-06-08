#ifndef ASSEMBELR_IR_H
#define ASSEMBELR_IR_H

// INSTRUCTION_TYPE aka InstructionType aka InstType
// defines the type of instruction we are dealing with, it contains enums for
// the types of instruction defined in GRAMMAR.md. More instructions may be
// added below for classification as a different INSTRUCTION
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

// INSTRUCTION_COMPONENT_TYPE
// type of componet used by an instruction, a componet is part of instructions
// holding crucial data regarding operation and operands of an instruction along
// with the information for validation correctness of an instruction
//
// -----------------------------------------------------------------------------+
// INSTRUCTION_COMPONENT_REGISTER_T         // register type                    |
// INSTRUCTION_COMPONENT_ADDRESS_T          // address type                     |
// INSTRUCTION_COMPONENT_IMMIDIATE_T        // immidiate type                   |
// -----------------------------------------------------------------------------+
typedef enum INSTRUCTION_COMPONENT_TYPE {
    INSTRUCTION_COMPONENT_REGISTER_T,
    INSTRUCTION_COMPONENT_ADDRESS_T,
    INSTRUCTION_COMPONENT_IMMIDIATE_T,
} InstructionComponentType, InstComponentType;

// INSTRUCTION_COMPONENT_REGISTER aka InstructionComponentRegister, InstRegister & Reg
// INSTRUCTION_COMPONENT_REGISTER is representation of a register operand within
// an instruction, holding its identifier and size to ensure compatibility.
typedef struct INSTRUCTION_COMPONENT_REGISTER {
    char* name;         // name of required register
    int   size;         // to check for size mismatches
} InstructionComponentRegister, InstRegister, Reg;


// INSTRUCTION_COMPONENT_ADDRESS aka InstructionComponentAddress, InstAddress & Adr
// INSTRUCTION_COMPONENT_ADDRESS represents a memory address operand, typically
// involving a base register and an associated offset value.
typedef struct INSTRUCTION_COMPONENT_ADDRESS {
    char* name;         // name of register with address
    char* offset;       // provided offset to address
} InstructionComponentAddress, InstAddress, Adr;


// INSTRUCTION_COMPONENT_IMMIDIATE aka InstructionComponentImmidiate, InstImmidiate & Imm
// INSTRUCTION_COMPONENT_IMMIDIATE represents an immediate value or a label used
// as a direct operand inside the assembly instruction.
typedef struct INSTRUCTION_COMPONENT_IMMIDIATE {
    char* value;        // if direct value was provided
    char* lable;        // if a lable was provided
} InstructionComponentImmidiate, InstImmidiate, Imm;


// INSTRUCTION_COMPONENT aka InstructionComponent & InstComponent
// INSTRUCTION_COMPONENT holds type of instruction component and associated data.
// 
// [ USAGE ] -------------------------------------------------------------------+
// INSTRUCTION_COMPONENT comp = __allocate;                                     |
// switch (comp.type){                                                          |
//      case INSTRUCTION_COMPONENT_REGISTER_T: return comp.as.reg;              |
//      case INSTRUCTION_COMPONENT_ADDRESS_T:  return comp.as.adr;              |
//      case INSTRUCTION_COMPONENT_IMMIDIATE_T: return comp.as.imm;             |
// }                                                                            |
// -----------------------------------------------------------------------------+
//
// [ NOTE ] --------------------------------------------------------------------+
// doing comp.as.<other> can fail with segmentation fault for comp.as.<type>    |
// -----------------------------------------------------------------------------+
typedef struct INSTRUCTION_COMPONENT {
    enum INSTRUCTION_COMPONENT_TYPE type;
    union {
        struct INSTRUCTION_COMPONENT_REGISTER    reg;
        struct INSTRUCTION_COMPONENT_ADDRESS     adr;
        struct INSTRUCTION_COMPONENT_IMMIDIATE   imm;
    } as;
} InstructionComponent, InstComponent;


// INSTRUCTION aka Instruction
// INSTRUCTION is representation of a fully parsed assembly instruction, holding 
// the mnemonic name alongside up to three operands (destination and sources).
typedef struct INSTRUCTION {
    char* name;
    struct INSTRUCTION_COMPONENT rd;
    struct INSTRUCTION_COMPONENT rs1;
    struct INSTRUCTION_COMPONENT rs2;
} Instruction;


#endif
