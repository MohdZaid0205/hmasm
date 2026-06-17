#ifndef ASSEMBLER_STRDUMP_H
#define ASSEMBLER_STRDUMP_H

#define NLINE "\n"
#define TLINE __LINE__
#define TFILE __FILE__
#define INSET "\t"
#define INSET2 INSET INSET

// argparse.h INVALID_FLAG_WARNING(flag)
#define ARGVS_IFW_DES "Assembler found an invalid flag: %('%s'%)" NLINE
#define ARGVS_IFW_LN1 "why? :raise:warning:=> %(INVALID_FLAG_WARNING%)" NLINE
#define ARGVS_IFW_END "Given flag %(%s%) will be %(IGNORED%)." NLINE NLINE

// argparse.h INVALID_FILE_CONTEXT_WARNING(type, file, extension, body)
#define ARGVS_IFCW_DES "assembler found an invalid file extension for %(%s%)" NLINE
#define ARGVS_IFCW_LN1 "while trying to resolve %(%s%) found extension %('%s'%)" NLINE
#define ARGVS_IFCW_LN2 "why? :raised:warning:=> %(INVALID_%s_FILE_EXTENSION_PROVIDED%)" NLINE
#define ARGVS_IFCW_LN3 "%([!]%) Allowed file extensions:" NLINE
#define ARGVS_IFCW_VOP "%d. %(%s%) : %s" NLINE
#define ARGVS_IFCW_NOP "%[<NO-OPTION-AVAILABLE>%] UPDATE %(%s%):%d" NLINE
#define ARGVS_IFCW_END "file: %(%s%) has been %(IGNORED%)" NLINE NLINE

// argparse.h INVALID_SRC_WARNING(file, extension)
#define ARGVS_ISW_O11 ".asm/.s" 
#define ARGVS_ISW_O21 ".asm.ir"
#define ARGVS_ISW_O12 "source file containing assembly code"
#define ARGVS_ISW_O22 "intermediate representation of assembly"

// argparse.h INVALID_OUT_WARNING(file, extension)
#define ARGVS_IOW_O11 ".exe/elf" 
#define ARGVS_IOW_O21 ".asm.ir "
#define ARGVS_IOW_O31 ".bin/raw"
#define ARGVS_IOW_O12 "final executable (platform deprendent)"
#define ARGVS_IOW_O22 "intermideate representation of AST"
#define ARGVS_IOW_O32 "raw binary as genrated by assembler"

// argparse.h INVALID_PRAM_WARNING(flag, pram, expect, action)
#define ARGVS_IPW_DES "assembler found an invalid parameter for flag %(%s%)" NLINE
#define ARGVS_IPW_LN1 "while trying to resolve for parameter, found %(%s%) expected %(%s%)" NLINE
#define ARGVS_IPW_LN2 "why? :raised:warning:=>%(INVALID_PRAMETER_PASSED%)" NLINE
#define ARGVS_IPW_END "flag has been %(%s%)" NLINE NLINE

// argparse.h INVALID_FORMAT_EXCEPTION(prodivded)
#define ARGVS_IFE_DES "assembler was provided with an invalid format=%(%s%)" NLINE
#define ARGVS_IFE_LN1 "while trying to resolve %(-[-f]ormat%) found %(%s%) which is not recognised as a format" NLINE
#define ARGVS_IFE_LN2 "why? :raised:exception:=>%(INVALID_FORMAT_EXCEPTION%)" NLINE
#define ARGVS_IFE_LN3 "%([!]%) Allowed output formats:" NLINE
#define ARGVS_IFE_VOP "%d. %(%s%) : %s" NLINE
#define ARGVS_IFE_NOP "%[<NO-OPTION-AVAILABLE>%] UPDATE %(%s%):%d" NLINE
#define ARGVS_IFE_NO1 "src/fmt/fmt.c"
#define ARGVS_IFE_NO2 "include/fmt.h"
#define ARGVS_IFE_END "maybe you forgot to %(compile:fmt-%s.c%) into %(hmasm%)?" NLINE NLINE

// argparse.h INVALID_ARCHITECTURE_EXCEPTION(prodivded)
#define ARGVS_IAE_DES "assembler was provided with an invalid isa=%(%s%)" NLINE
#define ARGVS_IAE_LN1 "while trying to resolve %(-[-i]sa%) found %(%s%) which is not recognised as a isa" NLINE
#define ARGVS_IAE_LN2 "why? :raised:exception:=>%(INVALID_ARCHITECTURE_EXCEPTION)" NLINE 
#define ARGVS_IAE_LN3 "%([!]%) Allowed output isa:" NLINE
#define ARGVS_IAE_VOP "%d. %(%s%) : %s" NLINE
#define ARGVS_IAE_NOP "%[<NO-OPTION-AVAILABLE>%] UPDATE %(%s%):%d" NLINE
#define ARGVS_IAE_NO1 "src/isa/isa.c"
#define ARGVS_IAE_NO2 "include/isa.h"
#define ARGVS_IAE_END "maybe you forgot to %(compile:isa-%s.c%) into %(hmasm%)?" NLINE NLINE

// argparse.h INVALID_PRAM_WARNING ~ GENERAL
#define ARGVS_NONE_PROVIDED "NONE"
#define ARGVS_PRAM_FNAME "<file-name>"
#define ARGVS_PRAM_FTYPE "<format-type>"
#define ARGVS_PRAM_ITYPE "<isa-type>"
#define ARGVS_RESP_FNAME "flag -[-o]ut IGNORED"
#define ARGVS_RESP_FTYPE "flag -[-f]ormat IGNORED"
#define ARGVS_RESP_ITYPE "flag -[-i]sa IGNORED"

// lexer.c LEXER_DEBUG
#define LEXER_DEBUG_DES "assembler successfully generated Lexer Tokens" NLINE
#define LEXER_DEBUG_END "lexer dump completed" NLINE

#define LEXER_DEBUG_LNO "  [L: %02d] "
#define LEXER_DEBUG_PUN "PUNCTUATION(%(\"%c\"%)) "
#define LEXER_DEBUG_OPR "OPERATION(%(\"%c\"%)) "
#define LEXER_DEBUG_LIT "LITERAL(%(\"%s\"%)) "
#define LEXER_DEBUG_WRD "WORD(%(\"%s\"%)) "

// parser.c UNEXPECTED_TOKEN_EXCEPTION
#define PARSER_UTE_DES "assembler encountered an unexpected token during parsing: %(%s%)" NLINE
#define PARSER_UTE_LN1 "while trying to resolve statement found %('%s'%) expected %('%s'%)" NLINE
#define PARSER_UTE_LN2 "why? :raised:exception:=> %(UNEXPECTED_TOKEN_EXCEPTION%)" NLINE
#define PARSER_UTE_LN3 "%([!]%) at line %d, character %d" NLINE
#define PARSER_UTE_END "statement parsing %(FAILED%) near %(%s%)" NLINE NLINE

// parser.c INVALID_DIRECTIVE_EXCEPTION
#define PARSER_IDE_DES "assembler encountered an invalid directive: %(%s%)" NLINE
#define PARSER_IDE_LN1 "while trying to resolve directive found %('%%%s'%) which is not recognized" NLINE
#define PARSER_IDE_LN2 "why? :raised:exception:=> %(INVALID_DIRECTIVE_EXCEPTION%)" NLINE
#define PARSER_IDE_LN3 "%([!]%) at line %d, character %d" NLINE
#define PARSER_IDE_END "directive parsing %(FAILED%) near %(%s%)" NLINE NLINE

// parser.c INVALID_DECLARATION_EXCEPTION
#define PARSER_DCL_DES "assembler encountered an invalid declaration: %(%s%)" NLINE
#define PARSER_DCL_LN1 "while parsing declaration found invalid structure" NLINE
#define PARSER_DCL_LN2 "why? :raised:exception:=> %(INVALID_DECLARATION_EXCEPTION%)" NLINE
#define PARSER_DCL_LN3 "%([!]%) at line %d, character %d" NLINE
#define PARSER_DCL_END "declaration parsing %(FAILED%) near %(%s%)" NLINE NLINE

// parser.c INVALID_INSTRUCTION_EXCEPTION
#define PARSER_IIE_DES "assembler encountered an invalid instruction mnemonic: %(%s%)" NLINE
#define PARSER_IIE_LN1 "while parsing instruction found unrecognized mnemonic %('%s'%)" NLINE
#define PARSER_IIE_LN2 "why? :raised:exception:=> %(INVALID_INSTRUCTION_EXCEPTION%)" NLINE
#define PARSER_IIE_LN3 "%([!]%) at line %d, character %d" NLINE
#define PARSER_IIE_END "instruction parsing %(FAILED%) near %(%s%)" NLINE NLINE


// ir.c IR_DEBUG_AST
#define IR_DEBUG_AST_DES "assembler successfully generated Abstract Syntax Tree (statements: %(%d%))" NLINE
#define IR_DEBUG_AST_END "ast dump completed" NLINE

#define IR_DEBUG_BLK_OPT "  [BLOCK: OPTIMIZATION] arch: %(%s%)" NLINE
#define IR_DEBUG_BLK_MAC "  [BLOCK: MACRO] name: %(%s%)" NLINE
#define IR_DEBUG_BLK_RAW "    raw data:" NLINE
#define IR_DEBUG_BLK_RLN "      | %s" NLINE

#define IR_DEBUG_DIR_SEC "  [DIRECTIVE: SECTION]" NLINE
#define IR_DEBUG_DIR_ALG "  [DIRECTIVE: ALIGN]" NLINE
#define IR_DEBUG_DIR_GLB "  [DIRECTIVE: GLOBAL]" NLINE
#define IR_DEBUG_DIR_EXT "  [DIRECTIVE: EXTERN]" NLINE
#define IR_DEBUG_DIR_ENT "  [DIRECTIVE: ENTRY]" NLINE
#define IR_DEBUG_DIR_INC "  [DIRECTIVE: INCLUDE]" NLINE
#define IR_DEBUG_DIR_ERR "  [DIRECTIVE: ERROR]" NLINE
#define IR_DEBUG_DIR_UNK "  [DIRECTIVE: UNKNOWN]" NLINE
#define IR_DEBUG_DIR_TRG "    target  : %(%s%)" NLINE
#define IR_DEBUG_DIR_MOD "    modifier: %(%s%)" NLINE

#define IR_DEBUG_DCL     "  [DECLARATION]" NLINE
#define IR_DEBUG_INS     "  [INSTRUCTION]" NLINE
#define IR_DEBUG_LBL     "  [LABEL: %(%s%)]" NLINE

// symtab.c IR_DEBUG_SYM
#define IR_DEBUG_SYM_DES "assembler successfully generated Symbol Table (symbols: %(%d%))" NLINE
#define IR_DEBUG_SYM_END "symbol table dump completed" NLINE

#define IR_DEBUG_SYM_LBL "  [LABEL] %(%s%) @ 0x%08lX" NLINE
#define IR_DEBUG_SYM_CON "  [CONST] %(%s%) (@%c) = 0x%lX" NLINE
#define IR_DEBUG_SYM_DAT "  [DATA ] %(%s%) (@%c) @ 0x%08lX (size: %lu bytes)" NLINE

#endif
