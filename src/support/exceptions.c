#include "exceptions.h"

void DEBUG_ASSERTION(bool cond, const char* expr, const char* desc) {
	if (!cond) 
		DEBUG("%(assertion failed%) while trying to assert cond=%(%s%)\n", {
			DEBUG_LN("\t", "expression for cond=%(%s%) returned %(%s%)\n",
				expr, cond ? "TRUE": "FALSE");
			DEBUG_LN("\t", "--> %(%s%)\n", desc );
			DEBUG_LN(""  , "No action taken on failed assertion as line "
				"was marked as %(DEBUG_ASSERTION%)\n");
		}, expr);
}

void WARNS_ASSERTION(bool cond, const char* expr, const char* desc) {
	if (!cond)
		WARNING("%(assertion failed%) while trying to assert cond=%(%s%)\n", {
			WARNING_LN("\t", "expression for cond=%(%s%) returned %(%s%)\n",
				expr, cond ? "TRUE" : "FALSE");
			WARNING_LN("\t", "--> %(%s%)\n", desc);
			WARNING_LN(""  , "No action taken on failed assertion as line "
				"was marked as %(WARNS_ASSERTION%)\n");
		}, expr);
}

void FATAL_ASSERTION(bool cond, const char* expr, const char* desc) {
	if (!cond)
		EXCEPTION("%(assertion failed%) while trying to assert cond=%(%s%)\n", {
			EXCEPTION_LN("\t", "expression for cond=%(%s%) returned %(%s%)\n",
				expr, cond ? "TRUE" : "FALSE");
			EXCEPTION_LN("\t", "--> %(%s%)\n", desc);
			EXCEPTION_LN(""  , "%(EXIT with -1%) on failed assertion as line "
				"was marked as %(FATAL_ASSERTION%)\n");
			exit(-1);
		}, expr);
}