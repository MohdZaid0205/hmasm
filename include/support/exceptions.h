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

// -----------------------------------------------------------------------------+
// {INFORMATION}: this is a statement explaining about information (one line)	|
//		<about> : <description>													|
// any peice of text is allowed here											|
// -----------------------------------------------------------------------------+
#define INFORMATION(what, body, ...){											\
	INF("%((INFORMATION)%):" );													\
	INF(what, __VA_ARGS__    );													\
	{ body }																	\
}

// in short idea boils down to displaying everything as required by user set of
// decoreated lines (one after another) in manner laid out as shown before. note
// you are allowd to add as many lines as you require and even pick default lines
// that you dont or you do require.


#endif
