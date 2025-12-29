#ifndef ASSEMBLER_EXCEPTIONS_H
#define ASSEMBLER_EXCEPTIONS_H

#include "pch.h"
#include "logging.h"

// [ INFO ] --------------------------------------------------------------------+
// exceptions.h contains a more refined exception printing and generation		|
// mechanism, that is illustreated in idea segment below, and more explicitly	|
// it aims to solve the unreasonable complex nature of writing exception that	|
// was introduced in previous version of this file. look at change-logs for		|
// more details on previous implementation.										|
// -----------------------------------------------------------------------------+


// [ CHANGE_LOG ] --------------------------------------------------------------+
// ---------------------------(28-12-2025 16:30)--------------------------------+
// Removed previous implementation and added information about upcoming feature |
// -----------------------------------------------------------------------------+
// ---------------------------(26-12-2025 16:30)--------------------------------+
// For previous implmentation of this file please Look at https://github.com/Moh|
// dZaid0205/hmasm/commits/e16c21de493d2a6f64a4cb78d94289b552ba621f/assembler/in|
// clude/exceptions.h															|
// -----------------------------------------------------------------------------+


// -----------------------------------------------------------------------------+
// { EXCEPTION }: this is a statement explaining about exception (one line)		|
//		<this is a line refrencing where exception came from>:<line>			|
//		this statement about exception and highlighting <EXCEPTION_TYPE>		|
//		... (other lines if you prefer)											|
// line explaining what to do if you experience this exception, this line is all|
// owed to be multiline. <detailed-fix-description>								|
// -----------------------------------------------------------------------------+
#define EXCEPTION(what, body, ...) {											\
	ERR("%([ EXCEPTION ]%): ");													\
	ERR(what, __VA_ARGS__	 );													\
	{ body }																	\
}
#define EXCEPTION_LN(inset,...) { ERR(inset); ERR(__VA_ARGS__); }
#define EXCEPTION_EN(...) { ERR("%(---> %)"); ERR(__VA_ARGS__); }

// -----------------------------------------------------------------------------+
// {  WARNING  }: this is a statement explaining about warning (one line)		|
//		<this is a line refrencing where exception came from>:<line>			|
//		this statement about warnings and highlighting <WARNING_TYPE>			|
//		... (other lines if you prefer)											|
// line explaining, why this warning is allowed to pass, and wat to do in order |
// to avoid this warning.														|
// -----------------------------------------------------------------------------+
#define WARNING(what, body, ...){												\
	WRN("%({  WARNING  }%): ");													\
	WRN(what, __VA_ARGS__    );													\
	{ body }																	\
}
#define WARNING_LN(inset,...) { WRN(inset); WRN(__VA_ARGS__); }
#define WARNING_EN(...) { WRN("%(---> %)"); WRN(__VA_ARGS__); }

// -----------------------------------------------------------------------------+
// {INFORMATION}: this is a statement explaining about information (one line)	|
//		<about> : <description>													|
// any peice of text is allowed here											|
// -----------------------------------------------------------------------------+
#define INFORMATION(what, body, ...){											\
	INF("%((INFORMATION)%): ");													\
	INF(what, __VA_ARGS__    );													\
	{ body }																	\
}
#define INFORMATION_LN(inset,...) { INF(inset); INF(__VA_ARGS__); }
#define INFORMATION_EN(...) { INF("%(---> %)"); INF(__VA_ARGS__); }

// -----------------------------------------------------------------------------+
// <   DEBUG   >: this is a statement explaining about information (one line)	|
//		<about> : <description>													|
// any peice of text is allowed here											|
// -----------------------------------------------------------------------------+
#define DEBUG(what, body, ...){													\
	DBG("%(<   DEBUG   >%): ");													\
	DBG(what, __VA_ARGS__    );													\
	{ body }																	\
}
#define DEBUG_LN(inset,...) { DBG(inset); DBG(__VA_ARGS__); }
#define DEBUG_EN(...) { DBG("%(---> %)"); DBG(__VA_ARGS__); }

// in short idea boils down to displaying everything as required by user set of
// decoreated lines (one after another) in manner laid out as shown before. note
// you are allowd to add as many lines as you require and even pick default lines
// that you dont or you do require.

// some default exceptions and assertion methods that have been tailored for this
// project, if this file is to be exported to other projects consider picking 
// specific functions that are required by other projects, or just ignore

void DEBUG_ASSERTION(bool cond, const char* expression, const char* description);
void WARNS_ASSERTION(bool cond, const char* expression, const char* description);
void FATAL_ASSERTION(bool cond, const char* expression, const char* description);

#endif
