#include "pch.h"
#include "exceptions.h"
#include "logging.h"
#include "argparse.h"
#include "lexer.h"
#include "isa.h"
#include "format.h"

int main(int argc, char** argv){
    
    DEBUG_ASSERTION(strcmp(assembler_isa_name, "i386") == 0, "this.isa==i386",
        "Check compatiblity of assembler with required isa defined as i386"
    );    
    WARNS_ASSERTION(strcmp(assembler_isa_name, "i386") == 0, "this.isa==i386",
        "Check compatiblity of assembler with required isa defined as i386"
    );    
    FATAL_ASSERTION(strcmp(assembler_isa_name, "i386") == 0, "this.isa==i386",
        "Check compatiblity of assembler with required isa defined as i386"
    );

    const char* source_file_name = NULL;
    const char* output_file_name = NULL;
    
    int i = 1;
    while (i < argc) {
        switch (_argparse_resolve_flag_type_for(argv[i])) {
        case ARGUMENT_OUT:
            break;
        case ARGUMENT_FMT:
            break;
        case ARGUMENT_HLP:
            break;
        case ARGUMENT_INP:
            break;
        case ARGUMENT_NONE:
            return -1;
        default:
            DBG("%(Aint No way that happened%)");
            return -100;
        }
        i++;
    }

    //if (source_file_name == NULL)
    //    exit(-1);
    //DBG("%[[ DEBUG ]%] looking for file name %(%s%)\n", source_file_name);

    //FILE* source = fopen(source_file_name, "r");
    //// exit(-1);

    //if (source == NULL)
    //    exit(-2);

    //unsigned int found = ' ';
    //while(found != EOF){
    //    found = fgetc(source);
    //    printf("%c", (unsigned char)found);
    //}

    return 0;
}
