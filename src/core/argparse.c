#include "argparse.h"
#include "exceptions.h"

enum ASSEMBLER_ARGUMENT_TYPE argparse_get_type_for(const char* flag){
    
    if (strcmp("-o", flag) == 0 || strcmp("--output" , flag) == 0)
        return ARGUMENT_OUT;
    if (strcmp("-f", flag) == 0 || strcmp("--format" , flag) == 0)
        return ARGUMENT_FMT;
    if (strcmp("-m", flag) == 0 || strcmp("--mapping", flag) == 0)
        return ARGUMENT_MAP;
    if (strcmp("-h", flag) == 0 || strcmp("--help"   , flag) == 0)
        return ARGUMENT_HLP;

    if (flag[0] == '-') {
        // TODO: raise WARNING about unused flag
        return ARGUMENT_NONE;
    }

    int size = strlen(flag);
    if ((flag[size-1] == 's' && flag[size-2] == '.'))
        return ARGUMENT_INP;
    if ((flag[size-1] == 'm' && flag[size-2] == 's' &&
         flag[size-3] == 'a' && flag[size-4] == '.'))
        return ARGUMENT_INP;

    // TODO: raise WARNING about unrecognized item in argument
    return ARGUMENT_NONE;
}

struct ASSEMBLER_ARGUMENT argparse_argument_input  (const char* value){
    return (struct ASSEMBLER_ARGUMENT) {
        .type= ARGUMENT_INP, .value= value,
    };
}
struct ASSEMBLER_ARGUMENT argparse_argument_output (const char* value){
    return (struct ASSEMBLER_ARGUMENT) {
        .type= ARGUMENT_OUT, .value= value,
    };
}
struct ASSEMBLER_ARGUMENT argparse_argument_format (const char* value){
    if((strcmp("bin",value) == 0 || 
        strcmp("elf",value) == 0 ||
        strcmp("exe",value) == 0 ) == false
    ) {
        // TODO: give EXCEPTION about incorrect format specfifed
        //ARGPARSE_FLAG_ARG_INCORRECT_EXCEPTION(value, "-[-f]ormat", 3,
        //    "bin", "to generate binary file containing direct translation",
        //    "elf", "containing debug information and symbols for script",
        //    "exe", "to generate a platform dependent executable for script"
        //);
    }

    return (struct ASSEMBLER_ARGUMENT){
        .type= ARGUMENT_FMT, .value= value,
    };
}
struct ASSEMBLER_ARGUMENT argparse_argument_mapping(const char* value){
    // FUNCTION_NOT_IMPLEMENTED_EXCEPTION(FNI_ARGUMENTS);
    // TODO: throw function not implemented exception
}
struct ASSEMBLER_ARGUMENT argparse_argument_help   (const char* value){
    return (struct ASSEMBLER_ARGUMENT){
        .type= ARGUMENT_HLP, .value= "HELP",
    };
}
