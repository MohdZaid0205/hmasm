#ifndef ASSEMBLER_STRDUMP_H
#define ASSEMBLER_STRDUMP_H

#define NLINE "\n"
#define TLINE __LINE__
#define TFILE __FILE__
#define INSET "\t"
#define INSET2 INSET INSET

// argparse.h INVALID_FLAG_WARNING(flag)
#define LEXER_IFW_DES "Assembler found an invalid flag: %('%s'%)" NLINE
#define LEXER_IFW_LN1 "why? :raise:warning:=> %(INVALID_FLAG_WARNING%)" NLINE
#define LEXER_IFW_END "Given flag will be %(IGNORED%)." NLINE NLINE

// argparse.h INVALID_FILE_CONTEXT_WARNING(type, file, extension, body)
#define LEXER_IFCW_DES "assembler found an invalid file extension for %(%s%)" NLINE
#define LEXER_IFCW_LN1 "while trying to resolve %(%s%) found extension %('%s'%)" NLINE
#define LEXER_IFCW_LN2 "why? :raised:warning:=> %(INVALID_%s_FILE_EXTENSION_PROVIDED%)" NLINE
#define LEXER_IFCW_LN3 "%([!]%) Allowed file extensions:" NLINE
#define LEXER_IFCW_VOP "%d. %(%s%) : %s" NLINE
#define LEXER_IFCW_NOP "%[<NO-OPTION-AVAILABLE>%] UPDATE %(%s%):%d" NLINE
#define LEXER_IFCW_END "file: %(%s%) has been %(IGNORED%)" NLINE NLINE

// argparse.h INVALID_SRC_WARNING(file, extension)
#define LEXER_ISW_O11 ".asm/.s" 
#define LEXER_ISW_O21 ".asm.ir"
#define LEXER_ISW_O12 "source file containing assembly code"
#define LEXER_ISW_O22 "intermediate representation of assembly"

// argparse.h INVALID_OUT_WARNING(file, extension)
#define LEXER_IOW_O11 ".exe/elf" 
#define LEXER_IOW_O21 ".asm.ir "
#define LEXER_IOW_O31 ".bin/raw"
#define LEXER_IOW_O12 "final executable (platform deprendent)"
#define LEXER_IOW_O22 "intermideate representation of AST"
#define LEXER_IOW_O32 "raw binary as genrated by assembler"

// argparse.h INVALID_PRAM_WARNING(file, extension)
#define LEXER_IPW_DES "assembler found an invalid parameter for flag %(%s%)" NLINE
#define LEXER_IPW_LN1 "while trying to resolve for parameter, found %(%s%) expected %(%s%)" NLINE
#define LEXER_IPW_LN2 "why? :raised:warning:=>%(INVALID_PRAMETER_PASSED%)" NLINE
#define LEXER_IPW_END "flag has been %(%s%)" NLINE NLINE

#endif