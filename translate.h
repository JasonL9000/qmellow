#pragma once

#include <memory>
#include "expr.h"
#include "lexer.h"
#include "parser.h"

namespace qmellow {

/* Translate a source text into a syntax tree. */
std::unique_ptr<expr_t> translate(const char *text);

}  // qmellow
