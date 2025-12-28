#include "pch.h"
#include "logging.h"
#include "argparse.h"
#include "lexer.h"
#include "isa.h"
#include "format.h"

int main(int argc, char** argv){
    
    DEBUG("This distribution compiles to ISA:%(%s%)\n", assembler_isa_name);
    DEBUG("This distribution compiles to FMT:%(%s%)\n", assembler_format_name);

    const char* source_file_name = NULL;
    const char* output_file_name = NULL;
    
    int i = 1;
    while (i < argc){
        switch (argparse_get_type_for(argv[i])){
            case ARGUMENT_OUT:
                output_file_name = argparse_argument_output(argv[++i]).value;
                break;
            case ARGUMENT_FMT:
                argparse_argument_format(argv[++i]);
                break;
            case ARGUMENT_MAP:
                argparse_argument_mapping(argv[++i]);
                break;
            case ARGUMENT_HLP:
                argparse_argument_help("HELP ME");
                break;
            case ARGUMENT_INP:
                source_file_name = argparse_argument_input(argv[i]).value;
                break;
            case ARGUMENT_NONE:
                return -1;
            default:
                DBG("%(Aint No way that happened%)");
                return -100;
        }
        i++;
    }

    if (source_file_name == NULL)
        exit(-1);
    DEBUG("looking for file name %(%s%)\n", source_file_name);

    FILE* source = fopen(source_file_name, "r");
    // exit(-1);

    if (source == NULL)
        exit(-2);

    unsigned int found = ' ';
    while(found != EOF){
        found = fgetc(source);
        printf("%c", (unsigned char)found);
    }

    return 0;
}
