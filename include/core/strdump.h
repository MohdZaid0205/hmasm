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



#endif
