#include "lexer.h"
#include "logging.h"
#include "exceptions.h"
#include "strdump.h"

// Implemetaion for _print_lexeme_[...]
void _print_lexeme_idn(struct LEXEME_IDENTIFIER *idn) {
    DEBUG(LEXER_PRINT_IDN, {}, LEXER_PRINT_ARG_STR(idn));
}
void _print_lexeme_pun(struct LEXEME_PUNCTUATION *pun) {
    DEBUG(LEXER_PRINT_PUN, {}, LEXER_PRINT_ARG_CHR(pun));
}
void _print_lexeme_key(struct LEXEME_KEYWORD* key) {
    DEBUG(LEXER_PRINT_KEY, {}, LEXER_PRINT_ARG_STR(key));
}
void _print_lexeme_lit(struct LEXEME_LITERAL* lit) {
    DEBUG(LEXER_PRINT_LIT, {}, LEXER_PRINT_ARG_STR(lit));
}
void _print_lexeme_opr(struct LEXEME_OPERATION* opr) {
    DEBUG(LEXER_PRINT_OPR, {}, LEXER_PRINT_ARG_CHR(opr));
}
void _print_lexeme_nul(struct LEXEME_NULL* nul) {
    DEBUG(LEXER_PRINT_NUL, {}, LEXER_PRINT_ARG_CHR(nul));
}

// Implementaion for print_lexeme
void _print_lexeme(struct LEXEME *lex) {
    switch (lex->type) {
        case LEXEME_IDN:
            _print_lexeme_idn(&lex->as.idn);
            break;
        case LEXEME_PUN:
            _print_lexeme_pun(&lex->as.pun);
            break;
        case LEXEME_KEY:
            _print_lexeme_key(&lex->as.key);
            break;
        case LEXEME_LIT:
            _print_lexeme_lit(&lex->as.lit);
            break;
        case LEXEME_OPR:
            _print_lexeme_opr(&lex->as.opr);
            break;
        case LEXEME_NUL:
            _print_lexeme_nul(&lex->as.nul);
            break;
        default:
            EXCEPTION(LEXER_PRINT_UKN, {
                EXCEPTION_LN(LEXER_PRINT_UKN_INF, "");
                EXCEPTION_EN(LEXER_PRINT_UKN_END, TFILE, TLINE);
            }, "");
            break;
    }
}

