#ifndef ASSEMBLER_LOGGING_H
#define ASSEMBLER_LOGGING_H

#include "pch.h"

// [ SCOPE ] -------------------------------------------------------------------+
// This file contains ways for showing elaborate exceptions and its trace in	|
// order to provide with better and more workable exceptions for assembler.		|
// -----------------------------------------------------------------------------+

// Logger Level Option(s) contains all supported options that that can be used
// selectively to create custom logging level as per your requirement by using
// bitwise OR operations such as (LEVEL_LOG | LEVEL_INF) shows LOGs and INFOs
typedef enum LOGGER_LEVEL_OPTION {
	LEVEL_NONE= 0b000000,		// NONE or NO		are allowed to pass.
	LEVEL_LOG = 0b000001,		// LOGs				are allowed to pass.
	LEVEL_INF = 0b000010,		// INFORMATIONs		are allowed to pass.
	LEVEL_DBG = 0b000100,		// DEBUGs			are allowed to pass.
	LEVEL_WRN = 0b001000,		// WARNINGs			are allowed to pass.
	LEVEL_ERR = 0b010000,		// ERRORs			are allowed to pass.
	LEVEL_FAT = 0b100000,		// FATALs			are allowed to pass.
	LEVEL_ALL = 0b111111,		// ALL				are allowed to pass.
} LogLevel, TraceLevel;

// [ INFO ] --------------------------------------------------------------------+
// In order to default to some output level if level has not been defined		|
// if build into DEBUG mode then set debug to ALL levels otherwise ERRORs		|
// -----------------------------------------------------------------------------+

#ifndef LOGGING_LEVEL
	#if defined(NDEBUG)
		#define LOGGING_LEVEL (LEVEL_WRN | LEVEL_ERR | LEVEL_FAT | LEVEL_INF)
	#else
		#define LOGGING_LEVEL (LEVEL_ALL)
	#endif
#endif

// allow optimization for statements that are never executed at runtime exclusion
#if defined(NDEBUG)
	#define OPTIMIZATIONS(level) if (level & LOGGING_LEVEL)
#else
	#define OPTIMIZATIONS(level) if (true)
#endif

// [ INFO ] --------------------------------------------------------------------+
// helper macro definitions for dealing with ANSI escape sequence for colors if	|
// no color mode is selected. in order to disable this coloring mechanism pass	|
// flag -DLOGGING_DISABLE_COLORING to compiler									|
// -----------------------------------------------------------------------------+

#ifndef LOGGING_DISABLE_COLORING
	// define macros to create string that contains ansi sequence for colors
	#define COLORED_FOREGROUND(r, g, b) "\033[38;2;"#r";"#g";"#b"m"
	#define COLORED_BACKGROUND(r, g, b) "\033[48;2;"#r";"#g";"#b"m"
	#define DEFAULT_FOREGROUND "\033[39m"
	#define DEFAULT_BACKGROUND "\033[49m" "\033[39m"
#else
	// define colors macros as empty so that no ansi is used at runtime
	#define COLORED_FOREGROUND(r, g, b) ""
	#define COLORED_BACKGROUND(r, g, b) ""
	#define DEFAULT_FOREGROUND ""
	#define DEFAULT_BACKGROUND ""
#endif

#ifndef CLIENT_COLORS
	// define all foreground colors that is to be used for corresponding level
	#define LOGGER_COLOR_LOG_F COLORED_FOREGROUND(127, 127, 127)
	#define LOGGER_COLOR_INF_F COLORED_FOREGROUND(127, 127, 255)
	#define LOGGER_COLOR_DBG_F COLORED_FOREGROUND(127, 255, 127)
	#define LOGGER_COLOR_WRN_F COLORED_FOREGROUND(255, 255, 127)
	#define LOGGER_COLOR_ERR_F COLORED_FOREGROUND(255, 127, 127)
	#define LOGGER_COLOR_FAT_F COLORED_FOREGROUND(255, 127, 000)

	// define all background colors that is to be used for corresponding level
	#define LOGGER_COLOR_LOG_B COLORED_BACKGROUND(127, 127, 127) COLORED_FOREGROUND(0,0,0)
	#define LOGGER_COLOR_INF_B COLORED_BACKGROUND(127, 127, 255) COLORED_FOREGROUND(0,0,0)
	#define LOGGER_COLOR_DBG_B COLORED_BACKGROUND(127, 255, 127) COLORED_FOREGROUND(0,0,0)
	#define LOGGER_COLOR_WRN_B COLORED_BACKGROUND(255, 255, 127) COLORED_FOREGROUND(0,0,0)
	#define LOGGER_COLOR_ERR_B COLORED_BACKGROUND(255, 127, 127) COLORED_FOREGROUND(0,0,0)
	#define LOGGER_COLOR_FAT_B COLORED_BACKGROUND(255, 127, 000) COLORED_FOREGROUND(0,0,0)
#else
	#if !defined(LOGGER_COLOR_LOG_F)|| \
		!defined(LOGGER_COLOR_INF_F)|| \
		!defined(LOGGER_COLOR_DBG_F)|| \
		!defined(LOGGER_COLOR_WRN_F)|| \
		!defined(LOGGER_COLOR_ERR_F)|| \
		!defined(LOGGER_COLOR_FAT_F)|| \
		!defined(LOGGER_COLOR_LOG_B)|| \
		!defined(LOGGER_COLOR_INF_B)|| \
		!defined(LOGGER_COLOR_DBG_B)|| \
		!defined(LOGGER_COLOR_WRN_B)|| \
		!defined(LOGGER_COLOR_ERR_B)|| \
		!defined(LOGGER_COLOR_FAT_B) 
		
		// do not remove line below under any condition, if you are accedentally
		// triggering this error at compile time please add the following code
		// before include this "logging.h" in your project.
	
		// [ CODE ]-------------------------------------------------------------+
		// #define LOGGER_COLOR_LOG_F "COLOR"									|
		// #define LOGGER_COLOR_INF_F "COLOR"									|
		// #define LOGGER_COLOR_DBG_F "COLOR"									|
		// #define LOGGER_COLOR_WRN_F "COLOR"									|
		// #define LOGGER_COLOR_ERR_F "COLOR"									|
		// #define LOGGER_COLOR_FAT_F "COLOR"									|
		// #define LOGGER_COLOR_LOG_B "COLOR"									|
		// #define LOGGER_COLOR_INF_B "COLOR"									|
		// #define LOGGER_COLOR_DBG_B "COLOR"									|
		// #define LOGGER_COLOR_WRN_B "COLOR"									|
		// #define LOGGER_COLOR_ERR_B "COLOR"									|
		// #define LOGGER_COLOR_FAT_B "COLOR"									|
		// ---------------------------------------------------------------------+

		// feel free to provide your own asscii escape sequence for these purpose
		// if rather your intentions were to remove coloring please see compiler
		// flag LOGGING_DISABLE_COLORING pass as -DLOGGING_DISABLE_COLORING

		#error "Not all LOGGER_COLOR_XXX_T has been defined, see definitions"
	#endif
#endif

// A helper method to get foreground color associated with LOGGER_LEVEL_OPTION
// in order to get LEVEL_XXX's color as defined by default ot as defined by usr
const char* __get_associated_foreground_for(enum LOGGER_LEVEL_OPTION level);

// A helper method to get background color associated with LOGGER_LEVEL_OPTION
// in order to get LEVEL_XXX's color as defined by default ot as defined by usr
const char* __get_associated_background_for(enum LOGGER_LEVEL_OPTION level);

// Option to select the stream to write into with respect to our FILE* stream
// LOGGING_STREAM_GENERAL_INTO to specify where to stream general messages to
// LOGGING_STREAM_EXCEPTION_INTO to specify where to stream error messages to
#ifndef LOG_STREAM_GEN_INTO
	#define LOG_STREAM_GEN_INTO stdout
#endif
#ifndef LOG_STREAM_EXC_INTO
	#define LOG_STREAM_EXC_INTO stderr
#endif

// Underlying method to display onto selected stream based on log level that
// has been selected at compile time in order to deal with size and instruction
// limitations, this is supposed to work with output files as it writes to
// any specified stream.
// 
// lfprintf defines:
//		- %[ : start background highlight
//		- %] : stops nackground highlight
//		- %( : start foreground highlight
//		- %) : stops foreground highlight
// 
// NOTE: opening and closing stream is not job of this method, if user passes any 
//		file as an input, they are required to make sure that it is open
int lfprintf(LogLevel level, FILE* stream, const char* format, ...);

// define macros required for main usage of logging and required application by
// user to abstract away unnecessary implementation details, along with other macro
// for ease of use and adding hinting options to all usages of those macros

#define LOG(...) OPTIMIZATIONS(LEVEL_LOG){lfprintf(LEVEL_LOG, LOG_STREAM_GEN_INTO, __VA_ARGS__);}
#define INF(...) OPTIMIZATIONS(LEVEL_INF){lfprintf(LEVEL_INF, LOG_STREAM_GEN_INTO, __VA_ARGS__);}
#define DBG(...) OPTIMIZATIONS(LEVEL_DBG){lfprintf(LEVEL_DBG, LOG_STREAM_GEN_INTO, __VA_ARGS__);}
#define WRN(...) OPTIMIZATIONS(LEVEL_WRN){lfprintf(LEVEL_WRN, LOG_STREAM_GEN_INTO, __VA_ARGS__);}
#define ERR(...) OPTIMIZATIONS(LEVEL_ERR){lfprintf(LEVEL_ERR, LOG_STREAM_EXC_INTO, __VA_ARGS__);}
#define FAT(...) OPTIMIZATIONS(LEVEL_FAT){lfprintf(LEVEL_FAT, LOG_STREAM_EXC_INTO, __VA_ARGS__);}

#define LOG_IF(condition, ...) if (condition) { LOG(__VA_ARGS__) }
#define INF_IF(condition, ...) if (condition) { INF(__VA_ARGS__) }
#define DBG_IF(condition, ...) if (condition) { DBG(__VA_ARGS__) }
#define WRN_IF(condition, ...) if (condition) { WRN(__VA_ARGS__) }
#define ERR_IF(condition, ...) if (condition) { ERR(__VA_ARGS__) }
#define FAT_IF(condition, ...) if (condition) { FAT(__VA_ARGS__) }


#endif
