#include "parser.h"
#include "exceptions.h"
#include "strdump.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// MACROS for error reporting
#define UNEXPECTED_TOKEN_EXCEPTION(expected, actual, lno, cno)                  \
    EXCEPTION(PARSER_UTE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_UTE_LN1, actual, expected);                  \
        EXCEPTION_LN(INSET, PARSER_UTE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_UTE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_UTE_END, actual);                                   \
    }, actual)

#define INVALID_DIRECTIVE_EXCEPTION(keyword, lno, cno)                          \
    EXCEPTION(PARSER_IDE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_IDE_LN1, keyword);                           \
        EXCEPTION_LN(INSET, PARSER_IDE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_IDE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_IDE_END, keyword);                                  \
    }, keyword)

#define INVALID_DECLARATION_EXCEPTION(context, lno, cno)                        \
    EXCEPTION(PARSER_DCL_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_DCL_LN1);                                    \
        EXCEPTION_LN(INSET, PARSER_DCL_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_DCL_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_DCL_END, context);                                  \
    }, context)

#define INVALID_INSTRUCTION_EXCEPTION(mnemonic, lno, cno)                       \
    EXCEPTION(PARSER_IIE_DES, {                                                 \
        EXCEPTION_LN(INSET, PARSER_IIE_LN1, mnemonic);                          \
        EXCEPTION_LN(INSET, PARSER_IIE_LN2);                                    \
        EXCEPTION_LN(INSET, PARSER_IIE_LN3, lno, cno);                          \
        EXCEPTION_EN(PARSER_IIE_END, mnemonic);                                 \
    }, mnemonic)

#ifdef PARSER_DEBUG
    // TODO: add debug statements
#endif


// Token Lookahead State
static struct LEXEME_TOKEN __lookahead;
static bool __has_lookahead = false;

// Safely fetches next token, using cached lookahead if available
bool get_next_token(FILE* source, struct LEXEME_TOKEN* result, bool raw_mode) {
    if (__has_lookahead && !raw_mode) {
        *result = __lookahead;
        __has_lookahead = false;
        return true;
    }
    return lexer(source, result, raw_mode);
}
