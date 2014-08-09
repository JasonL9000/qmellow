#include "translate.h"

using namespace std;
using namespace qmellow;

/* Translate a source text into a syntax tree. */
unique_ptr<expr_t> translate(const char *text) {
  return parser_t::parse(lexer_t::lex(text).data());
}
