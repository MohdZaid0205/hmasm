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
    enum INSTRUCTION_TYPE type;
    struct INSTRUCTION_COMPONENT rd;
    struct INSTRUCTION_COMPONENT rs1;
    struct INSTRUCTION_COMPONENT rs2;
} Instruction;


// DECLARATION_COMPONENT_TYPE
// type of declaration component used by a declaration, a component is part of 
// declarations holding crucial data regarding operation and values of a definition.
//
// -----------------------------------------------------------------------------+
// DECLARATION_COMPONENT_CONST_T            // constant definition              |
// DECLARATION_COMPONENT_DATA_T             // initialized data array           |
// DECLARATION_COMPONENT_RESERVE_T          // uninitialized memory reservation |
// -----------------------------------------------------------------------------+
typedef enum DECLARATION_COMPONENT_TYPE {
    DECLARATION_COMPONENT_CONST_T,
    DECLARATION_COMPONENT_DATA_T,
    DECLARATION_COMPONENT_RESERVE_T,
} DeclarationComponentType, DeclCompType;


// DECLARATION_COMPONENT_CONST aka DeclarationComponentConst & DeclCompConst
// DECLARATION_COMPONENT_CONST is representation of a constant declaration within
// assembly, holding a single evaluated immediate value for compile-time usage.
typedef struct DECLARATION_COMPONENT_CONST {
    struct INSTRUCTION_COMPONENT_IMMIDIATE value; // single evaluated value
} DeclarationComponentConst, DeclCompConst;


// DECLARATION_COMPONENT_DATA_VALUES aka DeclarationComponentDataValues & DeclCompDataVal
// DECLARATION_COMPONENT_DATA_VALUES represents an array of initialized values 
// that will be written sequentially into the target binary (e.g., .data section).
typedef struct DECLARATION_COMPONENT_DATA_VALUES {
    struct INSTRUCTION_COMPONENT_IMMIDIATE* values; // array of values
    int count;                                      // number of values in array
} DeclarationComponentDataValues, DeclCompDataVal;


// DECLARATION_COMPONENT_RESERVE aka DeclarationComponentReserve & DeclCompRes
// DECLARATION_COMPONENT_RESERVE represents a request to allocate uninitialized
// memory space inside the binary (e.g., .bss section) based on the target type.
typedef struct DECLARATION_COMPONENT_RESERVE {
    int size;                           // number of elements to reserve
} DeclarationComponentReserve, DeclCompRes;


// DECLARATION_DATA_TYPE
// defines the primitive data sizes available for allocation.
typedef enum DECLARATION_DATA_TYPE {
    DECLARATION_B_T,    // declaration for byte
    DECLARATION_W_T,    // declaration for word
    DECLARATION_D_T,    // declaration for double
    DECLARATION_Q_T,    // declaration for quadword
    DECLARATION_T_T,    // declaration for tenword
    DECLARATION_Y_T,    // declaration for Y-word
    DECLARATION_Z_T,    // declaration for Z-word
} DeclarationDataType, DeclDataType;


// DECLARATION aka Declaration & Decl
// DECLARATION holds type of declaration component and associated data.
// 
// [ USAGE ] -------------------------------------------------------------------+
// DECLARATION decl = __allocate;                                               |
// switch (decl.type){                                                          |
//      case DECLARATION_COMPONENT_CONST_T:   return decl.as.cnst;              |
//      case DECLARATION_COMPONENT_DATA_T:    return decl.as.vals;              |
//      case DECLARATION_COMPONENT_RESERVE_T: return decl.as.resv;              |
// }                                                                            |
// -----------------------------------------------------------------------------+
//
// [ NOTE ] --------------------------------------------------------------------+
// doing decl.as.<other> can fail with segmentation fault for decl.as.<type>    |
// -----------------------------------------------------------------------------+
typedef struct DECLARATION {
    enum DECLARATION_COMPONENT_TYPE type; // holds the type of current component
    enum DECLARATION_DATA_TYPE dt;        // target primitive type (e.g., @B, @W)
    char* name;                           // name assigned to this data/constant
    
    union {
        struct DECLARATION_COMPONENT_CONST       cnst; // read as constant
        struct DECLARATION_COMPONENT_DATA_VALUES vals; // read as data array
        struct DECLARATION_COMPONENT_RESERVE     resv; // read as reservation
    } as;
} Declaration, Decl;

#endif
