#ifndef ASSEMBELR_IR_H
#define ASSEMBELR_IR_H
#include <stdio.h>

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


// DIRECTIVE_TYPE
// type of directive used to instruct the assembler, representing meta-commands
// rather than executable CPU instructions.
//
// --------------+--------------------------------------------------------------+
// SECTION_T     | defines target memory segment (e.g., TEXT, DATA)             |
// ALIGN_T       | enforces byte boundary alignment for subsequent data/code    |
// GLOBAL_T      | exports a symbol for external linkage                        |
// EXTERN_T      | imports a symbol from an external source                     |
// ENTRY_T       | defines execution entry point of binary                      |
// INCLUDE_T     | file inclusion directive                                     |
// ERROR_T       | explicitly halts compilation with message                    |
// REGISTER..._T | assigns an alias to a hardware register                      |
// --------------+--------------------------------------------------------------+
typedef enum DIRECTIVE_TYPE {
    DIRECTIVE_SECTION_T,
    DIRECTIVE_ALIGN_T,
    DIRECTIVE_GLOBAL_T,
    DIRECTIVE_EXTERN_T,
    DIRECTIVE_ENTRY_T,
    DIRECTIVE_INCLUDE_T,
    DIRECTIVE_ERROR_T,
    DIRECTIVE_REGISTER_ALIAS_T
} DirectiveType, DirType;


// DIRECTIVE aka Directive & Dir
// DIRECTIVE is representation of an assembler directive, utilizing generic
// target and modifier strings to flexibly accommodate various directive schemas.
typedef struct DIRECTIVE {
    enum DIRECTIVE_TYPE type;           // type of directive we are handling
    char* target;                       // primary argument (e.g., "TEXT", "printf")
    char* modifier;                     // secondary argument (e.g., "[RWX]")
    int value;                          // numeric argument if required (e.g., align)
} Directive, Dir;


// BLOCK_TYPE
// defines the context type of a grouped block of statements.
//
// -----------------------------------------------------------------------------+
// BLOCK_MACRO_T        // block defining a reusable macro                      |
// BLOCK_OPTIMIZATION_T // block defining architecture specific optimizations   |
// -----------------------------------------------------------------------------+
typedef enum BLOCK_TYPE {
    BLOCK_MACRO_T,
    BLOCK_OPTIMIZATION_T,
} BlockType, BlkType;


// BLOCK_COMPONENT_MACRO aka BlockComponentMacro & BlkCompMac
// BLOCK_COMPONENT_MACRO represents metadata for a macro block definition, holding
// the name and list of parameters it accepts during expansion.
typedef struct BLOCK_COMPONENT_MACRO {
    char* name;                         // name of the macro
    char** pram;                        // array of parameter strings
    int npram;                          // number of parameters
} BlockComponentMacro, BlkCompMac;


// BLOCK_COMPONENT_OPTIMIZATION aka BlockComponentOptimization & BlkCompOpt
// BLOCK_COMPONENT_OPTIMIZATION represents metadata for an optimization block,
// holding the target architecture for which the contained code is intended.
typedef struct BLOCK_COMPONENT_OPTIMIZATION {
    char* arch;                         // target architecture (e.g., "RISC-V")
} BlockComponentOptimization, BlkCompOpt;


// BLOCK aka Block & Blk
// BLOCK holds type of block component, its metadata, and the raw unparsed body data.
// 
// [ USAGE ] -------------------------------------------------------------------+
// BLOCK blk = __allocate;                                                      |
// switch (blk.type){                                                           |
//      case BLOCK_MACRO_T:        return blk.as.mac;                           |
//      case BLOCK_OPTIMIZATION_T: return blk.as.opt;                           |
// }                                                                            |
// -----------------------------------------------------------------------------+
typedef struct BLOCK {
    enum BLOCK_TYPE type;               // holds the type of current block
    char* data;                         // raw unparsed string of block contents
    
    union {
        struct BLOCK_COMPONENT_MACRO        mac; // read as macro metadata
        struct BLOCK_COMPONENT_OPTIMIZATION opt; // read as optimization metadata
    } as;
} Block, Blk;


// STATEMENT_TYPE
// defines the type of statement we are dealing with, representing the absolute
// highest level of AST node generated by the parser.
//
// --------------+--------------------------------------------------------------+
// INSTRUCTION_T | executable cpu instruction mapped to internal IR             |
// DECLARATION_T | constant or data definitions mapped to target types          |
// DIRECTIVE_T   | assembler commands like sections, alignment, and linkage     |
// BLOCK_T       | grouped context holding raw sub-data                         |
// LABEL_T       | named marker representing a memory address                   |
// --------------+--------------------------------------------------------------+
typedef enum STATEMENT_TYPE {
    STATEMENT_INSTRUCTION_T,
    STATEMENT_DECLARATION_T,
    STATEMENT_DIRECTIVE_T,
    STATEMENT_BLOCK_T,
    STATEMENT_LABEL_T
} StatementType, StmtType;


// STATEMENT aka Statement & Stmt
// STATEMENT holds type of statement and associated specific AST data structure.
// 
// [ USAGE ] -------------------------------------------------------------------+
// STATEMENT stmt = __allocate;                                                 |
// switch (stmt.type){                                                          |
//      case STATEMENT_INSTRUCTION_T: return stmt.as.inst;                      |
//      case STATEMENT_DECLARATION_T: return stmt.as.decl;                      |
//      case STATEMENT_DIRECTIVE_T:   return stmt.as.dir;                       |
//      case STATEMENT_BLOCK_T:       return stmt.as.blk;                       |
//      case STATEMENT_LABEL_T:       return stmt.as.lable;                     |
// }                                                                            |
// -----------------------------------------------------------------------------+
//
// [ NOTE ] --------------------------------------------------------------------+
// doing stmt.as.<other> can fail with segmentation fault for stmt.as.<type>    |
// -----------------------------------------------------------------------------+
typedef struct STATEMENT {
    enum STATEMENT_TYPE type;           // holds the type of current statement
    
    union {
        struct INSTRUCTION inst;         // read as executable instruction
        struct DECLARATION decl;         // read as data declaration
        struct DIRECTIVE   dir;          // read as assembler directive
        struct BLOCK       blk;          // read as grouped block
        char*              lable;        // read as label address
    } as;
} Statement, Stmt;

// Debug dump functions for pretty printing AST components
void ir_dump_statement(struct STATEMENT* stmt);
void ir_dump_block(struct BLOCK* blk);
void ir_dump_directive(struct DIRECTIVE* dir);
void ir_dump_declaration(struct DECLARATION* decl);
void ir_dump_instruction(struct INSTRUCTION* inst);

struct AST;
void ir_dump_ast(struct AST* ast);
void ir_dump_lexer(FILE* source);

#endif
