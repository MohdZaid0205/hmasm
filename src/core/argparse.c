#include "argparse.h"
#include "exceptions.h"

#define INVALID_FLAG_WARNING(flag)												\
	WARNING("assembler found an invalid flag %(%s)%\n", {						\
		WARNING_LN("\t", "while trying to resolve flag type, found %(%s%)\n",	\
			flag);																\
		WARNING_LN("\t", "given flag is not valid:%(INVALID_FLAG_PASSED%)\n");	\
		WARNING_LN("\t", "%(IGNORING%) flag %(%s%), flag will not be used\n",	\
			flag);	\
	}, flag);

#define INVALID_FILE_WARNING(file, extension)									\
	WARNING("assembler found an invalid file extension %(%s)%\n", {				\
		WARNING_LN("\t", "while trying to resolve file type, found %(%s%)\n",	\
			file);																\
		WARNING_LN("\t", "given file is not valid:%(INVALID_FILE_PASSED%)\n");	\
		INFORMATION_LN("\t", "only allowed files are with extensions:\n");		\
		INFORMATION_LN("\t\t", "%(.asm%)/%(.s%) : source file containing asm"); \
		INFORMATION_LN("\t\t", "%(.asm.ir%) : intermideate represntation file");\
		WARNING_LN("\t", "%(IGNORING%) file %(%s%), file will not be used\n",	\
			file);																\
	}, extension);

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
		INVALID_FILE_WARNING(flag, "<NO_EXTENSION>");
		return ARGUMENT_NONE;
	}

	if (strcmp(extension, ".s"  ) == 0 || 
		strcmp(extension, ".asm") == 0 ||
		strcmp(extension, ".ir" ) == 0  )
		return ARGUMENT_INP;
	
	INVALID_FILE_WARNING(flag, extension);
	return ARGUMENT_NONE;
}
