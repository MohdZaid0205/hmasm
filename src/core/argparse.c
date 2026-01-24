#include "argparse.h"
#include "exceptions.h"
#include "strdump.h"

// WARNINGS and EXCEPTIONS (no external handler for these functions are required)

#define INVALID_FLAG_WARNING(flag)												\
	WARNING(LEXER_IFW_DES, {													\
		WARNING_LN(INSET, LEXER_IFW_LN1, flag);									\
		WARNING_EN(LEXER_IFW_END, flag);									    \
	}, flag);

#define INVALID_FILE_CONTEXT_WARNING(type, file, extension, body)				\
	WARNING(LEXER_IFCW_DES, {													\
		WARNING_LN(INSET, LEXER_IFCW_LN1, type, extension);						\
		WARNING_LN(INSET, LEXER_IFCW_LN2, type);								\
		INFORMATION_LN(INSET, LEXER_IFCW_LN3);									\
		{ body }																\
		WARNING_EN(LEXER_IFCW_END, file);										\
	}, type);

#define INVALID_SRC_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING("SOURCE", file, extension,						\
		INFORMATION_LN(INSET2, LEXER_IFCW_VOP, 1, LEXER_ISW_O11, LEXER_ISW_O12);\
		INFORMATION_LN(INSET2, LEXER_IFCW_VOP, 2, LEXER_ISW_O21, LEXER_ISW_O22);\
	)

#define INVALID_OUT_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING("OUTPUT", file, extension,						\
		INFORMATION_LN("\t\t", LEXER_IFCW_VOP, 1, LEXER_IOW_O11, LEXER_IOW_O12);\
		INFORMATION_LN("\t\t", LEXER_IFCW_VOP, 2, LEXER_IOW_O21, LEXER_IOW_O22);\
		INFORMATION_LN("\t\t", LEXER_IFCW_VOP, 3, LEXER_IOW_O31, LEXER_IOW_O32);\
	)

#define INVALID_PRAM_WARNING(flag, pram, expect, action)						\
	WARNING(LEXER_IPW_DES, {													\
		WARNING_LN(INSET, LEXER_IPW_LN1, pram, expect);							\
		WARNING_LN(INSET, LEXER_IPW_LN2);										\
		WARNING_EN(LEXER_IPW_END, action);										\
	}, flag);


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

	if (strcmp(flag, "-h") == 0 || strcmp(flag, "--help"  ) == 0) return ARGUMENT_HLP;
	if (strcmp(flag, "-o") == 0 || strcmp(flag, "--out"   ) == 0) return ARGUMENT_OUT;
	if (strcmp(flag, "-f") == 0 || strcmp(flag, "--format") == 0) return ARGUMENT_FMT;
	if (strcmp(flag, "-i") == 0 || strcmp(flag, "--isa"   ) == 0) return ARGUMENT_ISA;

	if (strcmp(flag, "--into-ir") == 0) return ARGUMENT_TIR;
	if (strcmp(flag, "--from-ir") == 0) return ARGUMENT_FIR;

	if (strncmp(flag, "-", 1) == 0)
		goto _warn_about_flags;

	char* extension = strrchr(flag, '.');
	
	if (extension == NULL) 
		goto _warn_about_extension;

	if (strcmp(extension, ".s"  ) == 0 || 
		strcmp(extension, ".asm") == 0 ||
		strcmp(extension, ".ir" ) == 0  )
		return ARGUMENT_INP;
	
_warn_about_extension:
	INVALID_SRC_WARNING(flag, extension ? extension : "<NULL>");
	return ARGUMENT_NONE;

_warn_about_flags:
	INVALID_FLAG_WARNING(flag);
	return ARGUMENT_NONE;
}


bool _argparse_parse_against_source_file(const char* string) {
	char* extension = strrchr(string, '.');

	if (extension == NULL)
		goto _warn_invalid_source;

	if (strcmp(extension, ".s"  ) == 0 ||
		strcmp(extension, ".asm") == 0 ||
		strcmp(extension, ".ir" ) == 0 )
		return true;

_warn_invalid_source:
	INVALID_SRC_WARNING(string, extension ? extension : "<NULL>");
	return false;
}

bool _argparse_parse_against_output_file(const char* string) {
	
	if (strncmp(string, "-", 1) == 0)
		goto _warn_invalid_pram;
	
	char* extension = strrchr(string, '.');

	if (extension == NULL)
		goto _warn_about_extension;

	if (strcmp(extension, ".exe") == 0 ||
		strcmp(extension, ".elf") == 0 ||
		strcmp(extension, ".bin") == 0 ||
		strcmp(extension, ".ir" ) == 0 )
		return true;

_warn_about_extension:
	INVALID_OUT_WARNING(string, extension ? extension : "<NULL>");
	return false;

_warn_invalid_pram:
	INVALID_PRAM_WARNING("-[-o]ut", string, "OUTPUT FILE",
		"PARAMETER < > and FLAG -[-o]ut has been IGNORED");
	return false;
}

bool _argparse_parse_against_fmt_type(const char* string);
bool _argparse_parse_against_isa_type(const char* string);
bool _argparse_parse_against_req_help(const char* string);
bool _argparse_parse_against_asm_into_iR(const char* string);
bool _argparse_parse_against_asm_from_iR(const char* string);
