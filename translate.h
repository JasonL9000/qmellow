#pragma once

#include <memory>
#include "expr.h"
#include "lexer.h"
#include "parser.h"

namespace qmellow {

/* Translate a source text into a syntax tree. */
inline std::unique_ptr<expr_t> translate(const char *text) {
  return parser_t::parse(lexer_t::lex(text).data());
}

}  // qmellow
