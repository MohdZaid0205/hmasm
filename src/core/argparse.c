#include "argparse.h"
#include "exceptions.h"

// WARNINGS and EXCEPTIONS (no external handler for these functions are required)

#define INVALID_FLAG_WARNING(flag)												\
	WARNING("assembler found an invalid flag %(%s)%\n", {						\
		WARNING_LN("\t", "while trying to resolve flag type, found %(%s%)\n",	\
			flag);																\
		WARNING_LN("\t", "given flag is not valid:%(INVALID_FLAG_PASSED%)\n");	\
		WARNING_LN("->", "%(IGNORING%) flag %(%s%), flag will not be used\n",	\
			flag);	\
	}, flag);

#define INVALID_FILE_CONTEXT_WARNING(file, extension, context, allowed_msg)		\
	WARNING("assembler found an invalid file extension %(%s)%\n", {				\
		WARNING_LN("\t", "while trying to resolve %s, found %(%s%)\n",			\
			context, file);														\
		WARNING_LN("\t", "given file is not valid:%(INVALID_FILE_EXT%)\n");		\
		INFORMATION_LN("\t", "only allowed files are with extensions:\n");		\
		allowed_msg																\
		WARNING_LN("->", "%(IGNORING%) file %(%s%), file will not be used\n",	\
			file);																\
	}, extension);

#define INVALID_SRC_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING(file, extension, "input file type",			\
		INFORMATION_LN("\t\t", "%(.asm%)/%(.s%) : source file containing asm"); \
		INFORMATION_LN("\t\t", "%(.asm.ir%) : intermediate representation");	\
	)

#define INVALID_OUT_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING(file, extension, "output file type",			\
		INFORMATION_LN("\t\t", "%(.exe%) : executable file with proper fmt\t"); \
		INFORMATION_LN("\t\t", "%(.elf%) : (linux only) output for elf fmt\n"); \
		INFORMATION_LN("\t\t", "%(.bin%) : binary (directly compiled!) fmt\n"); \
		INFORMATION_LN("\t\t", "%(.asm.ir%) : intermediate representation");	\
	)

#define INVALID_PRAM_WARNING(flag, pram, expect, action)						\
	WARNING("assembler found an invalid parameter for flag %(%s)%\n", {			\
		WARNING_LN("\t", "while trying to resolve pram, found %(%s%) expected " \
			"%(%s%)\n", pram, expect);											\
		WARNING_LN("\t", "given argument is not valid:%(INVALID_PRAM%)\n");		\
		WARNING_LN("->", "%(%s%)\n", action);									\
	}, flag);

#define INVALID_OPTION_WARNING(flag, value, valid_options)						\
	WARNING("assembler found an invalid option value %(%s)%\n", {				\
		WARNING_LN("\t", "for flag %(%s%), found value %(%s%)\n", flag, value);	\
		INFORMATION_LN("\t", "supported options are:\n");						\
		valid_options															\
		WARNING_LN("->", "%(IGNORING%) setting, using default configuration\n");\
	}, value);


// Global VARIABLES defined to store result for argparse (globally)
char* _argparse_source_file = NULL;
char* _argparse_output_file = NULL;
char* _argparse_fmt_type	= NULL;        // DEPRICATED: just for compatiblity
char* _argparse_isa_type	= NULL;        // DEPRICATED: just for compatiblity
bool  _argparse_req_help	= false;
bool  _argparse_asm_into_iR = false;
bool  _argparse_asm_from_iR = false;

// FUNCTION definitions for private and public functions defined in argparse.h

enum ASSEMBLER_ARGUMENT_TYPE _argparse_resolve_flag_type_for(const char* flag) {

	if (strcmp(flag, "-h") == 0 || strcmp(flag, "--help"  ) == 0)
		return ARGUMENT_HLP;
	if (strcmp(flag, "-o") == 0 || strcmp(flag, "--out"   ) == 0)
		return ARGUMENT_OUT;
	if (strcmp(flag, "-f") == 0 || strcmp(flag, "--format") == 0)
		return ARGUMENT_FMT;
	if (strcmp(flag, "-i") == 0 || strcmp(flag, "--isa"   ) == 0)
		return ARGUMENT_ISA;

	if (strcmp(flag, "--into-ir") == 0)
		return ARGUMENT_TIR;
	if (strcmp(flag, "--from-ir") == 0)
		return ARGUMENT_FIR;

	if (strncmp(flag, "-", 1) == 0){
		INVALID_FLAG_WARNING(flag);
		return ARGUMENT_NONE;
	}

	char* extension = strrchr(flag, '.');
	
	if (extension == NULL) {
		INVALID_SRC_WARNING(flag, "<NO_EXTENSION>");
		return ARGUMENT_NONE;
	}

	if (strcmp(extension, ".s"  ) == 0 || 
		strcmp(extension, ".asm") == 0 ||
		strcmp(extension, ".ir" ) == 0  )
		return ARGUMENT_INP;
	
	INVALID_SRC_WARNING(flag, extension);
	return ARGUMENT_NONE;
}


bool _argparse_parse_against_source_file(const char* string) {
	char* extension = strrchr(string, '.');

	if (extension == NULL) {
		INVALID_SRC_WARNING(string, "<NO_EXTENSION>");
		return false;
	}

	if (strcmp(extension, ".s"  ) == 0 ||
			strcmp(extension, ".asm") == 0 ||
		strcmp(extension, ".ir" ) == 0 )
		return true;

	INVALID_SRC_WARNING(string, extension);
	return false;
}

bool _argparse_parse_against_output_file(const char* string) {
	
	if (strncmp(string, "-", 1) == 0) {
		INVALID_PRAM_WARNING("-[-o]ut", string, "OUTPUT FILE",
			"PARAMETER < > and FLAG -[-o]ut has been IGNORED");
		return false;
	}
	
	char* extension = strrchr(string, '.');

	if (extension == NULL) {
		INVALID_OUT_WARNING(string, "<NO_EXTENSION>");
		return false;
	}

	if (strcmp(extension, ".exe") == 0 ||
		strcmp(extension, ".elf") == 0 ||
		strcmp(extension, ".bin") == 0 ||
		strcmp(extension, ".ir" ) == 0 )
		return true;

	INVALID_OUT_WARNING(string, extension);
	return false;
}

bool _argparse_parse_against_fmt_type(const char* string);
bool _argparse_parse_against_isa_type(const char* string);
bool _argparse_parse_against_req_help(const char* string);
bool _argparse_parse_against_asm_into_iR(const char* string);
bool _argparse_parse_against_asm_from_iR(const char* string);
