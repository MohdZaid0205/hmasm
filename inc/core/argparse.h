#ifndef ASSEMBLER_ARGPARSE_H
#define ASSEMBLER_ARGPARSE_H

#include "pch.h"

// [ INFO ] --------------------------------------------------------------------+
// Argument parser for the assembler. This module is responsible for abstracting|
// the command line arguments (argv/argc) into a structured format that the     |
// main assembler loop can consume easily.                                      |
//                                                                              |
// It handles identifying flags (short `-o` or long `--output`), mapping them   |
// to their enumerated types, and associating them with their raw string values.|
// -----------------------------------------------------------------------------+

// [ IDEA ] --------------------------------------------------------------------+
// The main loop shouldn't care about string comparison for flags. It should    |
// operate on a stream of "Argument Objects". We define a structure that holds  |
// both the type of the argument (what it is) and the value (the data).         |
//                                                                              |
// We use a Factory Pattern approach here:                                      |
//      argparse_argument_input("file.s")   -> returns{ ARGUMENT_INP, "file.s" }|
//      argparse_argument_output("out.bin") -> returns{ ARGUMENT_OUT, "out.bin"}|
//                                                                              |
// Using this we populate some predefined variables that will be used by ##main |
// -----------------------------------------------------------------------------+

extern const char* _argparse_source_file;
extern const char* _argparse_output_file;
extern const char* _argparse_fmt_type;        // DEPRICATED: just for compatiblity
extern const char* _argparse_isa_type;        // DEPRICATED: just for compatiblity
extern bool        _argparse_req_help;
extern bool        _argparse_asm_into_iR;
extern bool        _argparse_asm_from_iR;

// DEPRICATED parameters may make a comeback as i have not fixated on dynamically
// or statically embed choices for architecture mapping and format specifications

// [ CHANGE_LOG ] --------------------------------------------------------------+
// ---------------------------(30-01-2026 17:00)--------------------------------+
// -> Implemented all functionlities as specified by previous iterations        |
// -> Decided that i would not nuke this shit in future (apparently)            |
// ---------------------------(28-12-2025 20:00)--------------------------------+
// -> Moved variables to argparse itself, variables are to be dealt within      |
//    argparse in order to hide complexity that argparse enforces.              |
// -> I might as well nuke this shit in future.                                 |
// -----------------------------------------------------------------------------+
// ---------------------------(26-12-2025 08:30)--------------------------------+
// -> Initial definition of argument types and structures.                      |
// -> Added constructors for specific argument types to ensure type safety      |
//    when creating argument structs manually.                                  |
// -----------------------------------------------------------------------------+

// enum ASSEMBLER_ARGUMENT_TYPE aka ArgumentType & FlagType
// Defines the specific role of a command line argument.
//
// -----------------+-----------------------------------------------------------+
// ARGUMENT_INP     | The input source file (.asm, .s). If no flag is provided  |
//                  | before a string, it is usually assumed to be this.        |
// ARGUMENT_OUT     | Destination file. Corresponds to -o or --output.          |
// ARGUMENT_FMT     | Output format (binary, executable, object). -f or --fmt.  |
// ARGUMENT_ISA     | Architecture mapping/layout file. -m or --map.            |
// ARGUMENT_HLP     | Request for help/usage text. -h or --help.                |
// ARGUMENT_FIR     | Input is IR. Assemble from Intermediate Representation.   |
// ARGUMENT_TIR     | Output IR. Dump Intermediate Representation to file.      |
// -----------------+-----------------------------------------------------------+
typedef enum ASSEMBLER_ARGUMENT_TYPE {
    ARGUMENT_INP,                   // Input file path (source)
    ARGUMENT_OUT,                   // Output file path (destination)
    ARGUMENT_FMT,                   // Format specifier (bin/exe/obj)
    ARGUMENT_ISA,                   // Memory mapping/Arch specifier
    ARGUMENT_HLP,                   // Help / Usage info
    ARGUMENT_FIR,                   // From IR: Treat input as intermediate rep
    ARGUMENT_TIR,                   // To IR: Stop after parsing, export IR
    ARGUMENT_NONE,                  // no type matched with argument
} ArgumentType, FlagType;

// _argparse_resolve_flag_type_for
// Internal helper to determine the Enum type of a specific string flag.
//
// [ NOTE ] --------------------------------------------------------------------+
// Distinct from the public API, this is the internal resolution logic used by  |
// the main parsing loop to decide how to handle the current token in argv.     |
// -----------------------------------------------------------------------------+
enum ASSEMBLER_ARGUMENT_TYPE _argparse_resolve_flag_type_for(const char* flag);

// [ INFO ] --------------------------------------------------------------------+
// The following functions are boolean predicates. They validate if a specific  |
// string input meets the criteria for a given argument category (e.g., does    |
// the source file exist? is the format string a valid supported type?).        |
// -----------------------------------------------------------------------------+

bool _argparse_parse_against_source_file(const char* string);
bool _argparse_parse_against_output_file(const char* string);
bool _argparse_parse_against_fmt_type(const char* string);
bool _argparse_parse_against_isa_type(const char* string);
bool _argparse_parse_against_req_help(const char* string);
bool _argparse_parse_against_asm_into_iR(const char* string);
bool _argparse_parse_against_asm_from_iR(const char* string);

// [ INFO ] --------------------------------------------------------------------+
// The following functions are the 'Setters'. They are responsible for the      |
// side-effect of modifying the global variables (_argparse_source_file, etc.)  |
// once a flag and its value have been successfully identified and validated.   |
// -----------------------------------------------------------------------------+

void _argparse_action_against_source_file(const char* string);
void _argparse_action_against_output_file(const char* string);
void _argparse_action_against_fmt_type(const char* string);
void _argparse_action_against_isa_type(const char* string);
void _argparse_action_against_req_help(const char* string);
void _argparse_action_against_asm_into_iR(const char* string);
void _argparse_action_against_asm_from_iR(const char* string);

// [ INFO ] --------------------------------------------------------------------+
// These functions are invoked when specific flags are missing from the command |
// line. They populate the global state with sensible defaults (e.g., setting   |
// output to "a.out" if -o is not provided).                                    |
// -----------------------------------------------------------------------------+

void _argparse_default_action_against_source_file();
void _argparse_default_action_against_output_file();
void _argparse_default_action_against_fmt_type();
void _argparse_default_action_against_isa_type();
void _argparse_default_action_against_req_help();
void _argparse_default_action_against_asm_into_iR();
void _argparse_default_action_against_asm_from_iR();

// argparse
// The main entry point for the argument parsing module.
void argparse(int argc, char** argv);

#endif
