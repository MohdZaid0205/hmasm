#include "argparse.h"
#include "exceptions.h"
#include "fmt.h"
#include "isa.h"
#include "strdump.h"

// WARNINGS and EXCEPTIONS (no external handler for these functions are required)

#define INVALID_FLAG_WARNING(flag)												\
	WARNING(ARGVS_IFW_DES, {													\
		WARNING_LN(INSET, ARGVS_IFW_LN1, flag);									\
		WARNING_EN(ARGVS_IFW_END, flag);									    \
	}, flag)

#define INVALID_FILE_CONTEXT_WARNING(type, file, extension, body)				\
	WARNING(ARGVS_IFCW_DES, {													\
		WARNING_LN(INSET, ARGVS_IFCW_LN1, type, extension);						\
		WARNING_LN(INSET, ARGVS_IFCW_LN2, type);								\
		INFORMATION_LN(INSET, ARGVS_IFCW_LN3);									\
		{ body }																\
		WARNING_EN(ARGVS_IFCW_END, file);										\
	}, type)

#define INVALID_SRC_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING("SOURCE", file, extension,						\
		INFORMATION_LN(INSET2, ARGVS_IFCW_VOP, 1, ARGVS_ISW_O11, ARGVS_ISW_O12);\
		INFORMATION_LN(INSET2, ARGVS_IFCW_VOP, 2, ARGVS_ISW_O21, ARGVS_ISW_O22);\
	)

#define INVALID_OUT_WARNING(file, extension)									\
	INVALID_FILE_CONTEXT_WARNING("OUTPUT", file, extension,						\
		INFORMATION_LN("\t\t", ARGVS_IFCW_VOP, 1, ARGVS_IOW_O11, ARGVS_IOW_O12);\
		INFORMATION_LN("\t\t", ARGVS_IFCW_VOP, 2, ARGVS_IOW_O21, ARGVS_IOW_O22);\
		INFORMATION_LN("\t\t", ARGVS_IFCW_VOP, 3, ARGVS_IOW_O31, ARGVS_IOW_O32);\
	)

#define INVALID_PRAM_WARNING(flag, pram, expect, action)						\
	WARNING(ARGVS_IPW_DES, {													\
		WARNING_LN(INSET, ARGVS_IPW_LN1, pram, expect);							\
		WARNING_LN(INSET, ARGVS_IPW_LN2);										\
		WARNING_EN(ARGVS_IPW_END, action);										\
	}, flag)

#define INVALID_FORMAT_EXCEPTION(provided)                                      \
    EXCEPTION(ARGVS_IFE_DES, {                                                  \
        EXCEPTION_LN(INSET, ARGVS_IFE_LN1, provided);                           \
        EXCEPTION_LN(INSET, ARGVS_IFE_LN2);                                     \
        INFORMATION_LN(INSET, ARGVS_IFE_LN3);                                   \
        if (supported_fmt_count == 0){                                          \
            INFORMATION_LN(INSET2, ARGVS_IFE_NOP, ARGVS_IFE_NO1, ARGVS_IFE_NO2);\
        }                                                                       \
        for (int i=0; i < supported_fmt_count; i++){                            \
            INFORMATION_LN(                                                     \
                INSET2, ARGVS_IFE_VOP, i,                                       \
                supported_fmt_array[i]->name,                                   \
                supported_fmt_array[i]->desc                                    \
            );                                                                  \
        }                                                                       \
        EXCEPTION_EN(ARGVS_IFE_END, provided);                                  \
    }, provided)

#define INVALID_ARCHITECTURE_EXCEPTION(provided)                                \
    EXCEPTION(ARGVS_IAE_DES, {                                                  \
        EXCEPTION_LN(INSET, ARGVS_IAE_LN1, provided);                           \
        EXCEPTION_LN(INSET, ARGVS_IAE_LN2);                                     \
        INFORMATION_LN(INSET, ARGVS_IAE_LN3);                                   \
        if (supported_isa_count == 0){                                          \
            INFORMATION_LN(INSET2, ARGVS_IAE_NOP, ARGVS_IAE_NO1, ARGVS_IAE_NO2);\
        }                                                                       \
        for (int i=0; i < supported_isa_count; i++){                            \
            INFORMATION_LN(                                                     \
                INSET2, ARGVS_IAE_VOP, i,                                       \
                supported_isa_array[i]->name,                                   \
                supported_isa_array[i]->desc                                    \
            );                                                                  \
        }                                                                       \
        EXCEPTION_EN(ARGVS_IAE_END, provided);                                  \
    }, provided)



// Global VARIABLES defined to store result for argparse (globally)
const char* _argparse_source_file	= NULL;
const char* _argparse_output_file	= NULL;
const char* _argparse_fmt_type		= NULL;
const char* _argparse_isa_type		= NULL;
bool		_argparse_req_help		= false;
bool		_argparse_asm_into_iR	= false;
bool		_argparse_asm_from_iR	= false;
bool		_argparse_raised_panic	= false;	// speacial flag for exit respone


// -----------------------------------------------------------------------------+
// FUNCTION definitions for private and public functions defined in argparse.h	|
// -----------------------------------------------------------------------------+

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

// -----------------------------------------------------------------------------+
// FUNCTION to parse AGAINST a specific type of argument						|
// -----------------------------------------------------------------------------+

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

bool _argparse_parse_against_fmt_type(const char* string){
    
    for (int i=0; i < supported_fmt_count; i++){
        if (strcmp(string, supported_fmt_array[i]->name) == 0)
            return true;
    }

_exit_invalid_format:
    INVALID_FORMAT_EXCEPTION(string);
	_argparse_raised_panic = true;
    return false;
}

bool _argparse_parse_against_isa_type(const char* string){
    
    for (int i=0; i < supported_isa_count; i++){
        if (strcmp(string, supported_isa_array[i]->name) == 0)
            return true;
    }

_exit_invalid_isa:
    INVALID_ARCHITECTURE_EXCEPTION(string);
	_argparse_raised_panic = true;
    return false;
}

bool _argparse_parse_against_req_help(const char* string){
    DEBUG("%(GUIDE%) has not been implemented yet\n", {}, "");
    return true;
}

bool _argparse_parse_against_asm_into_iR(const char* string){
    return true;
}

bool _argparse_parse_against_asm_from_iR(const char* string){
    return true;
}

// -----------------------------------------------------------------------------+
// FUNCTION to take action AGAINST a specific type of argument					|
// -----------------------------------------------------------------------------+

void _argparse_action_against_source_file(const char* string){
    if (_argparse_parse_against_source_file(string))
        _argparse_source_file = string;
}
void _argparse_action_against_output_file(const char* string){
    if (_argparse_parse_against_output_file(string))
        _argparse_output_file = string;
}
void _argparse_action_against_fmt_type(const char* string){
    if (_argparse_parse_against_fmt_type(string))
        _argparse_fmt_type = string;
}
void _argparse_action_against_isa_type(const char* string){
    if (_argparse_parse_against_isa_type(string))
        _argparse_isa_type = string;
}
void _argparse_action_against_req_help(const char* string){
    if (_argparse_parse_against_req_help(string))
        _argparse_req_help = true;
}
void _argparse_action_against_asm_into_iR(const char* string){
    if (_argparse_parse_against_asm_into_iR(string))
        _argparse_asm_into_iR = true;
}
void _argparse_action_against_asm_from_iR(const char* string){
    if (_argparse_parse_against_asm_from_iR(string))
        _argparse_asm_from_iR = true;
}

// -----------------------------------------------------------------------------+
// FUNCTION to take default action AGAINST a specific argument					|
// -----------------------------------------------------------------------------+

void _argparse_default_action_against_source_file() {
	if (!_argparse_source_file)
		_argparse_source_file = "a.asm";
}

void _argparse_default_action_against_output_file() {
	if (!_argparse_output_file)
		_argparse_output_file = "a.bin";
}

void _argparse_default_action_against_fmt_type() {
	if (!_argparse_fmt_type)
		exit(-1);
}

void _argparse_default_action_against_isa_type() {
	if (!_argparse_isa_type)
		exit(-1);
}

void _argparse_default_action_against_req_help() {
	// Default is false (global variable init), nothing to do here.
	// If you wanted to auto-print help on empty args, you would do it here.
}

void _argparse_default_action_against_asm_into_iR() {
	// Default is false, nothing to do.
}

void _argparse_default_action_against_asm_from_iR() {
	// Default is false, nothing to do.
}
