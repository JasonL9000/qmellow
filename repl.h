#pragma once

#include <memory>
#include <vector>
#include "expr.h"
#include "file.h"
#include "token.h"

namespace qmellow {

  class repl_t final {
    public:

    /* Open the given subject files. */
    explicit repl_t(const std::vector<std::string> &paths);

    /* Run forever, prompting the user for queries. */
    int operator()();

    private:

    /* Get an expression from the console. */
    static std::unique_ptr<expr_t> prompt_for_expr();

    /* Get tokens from the console. */
    static std::vector<token_t> prompt_for_tokens();

    /* The subject files. */
    std::vector<file_t> files;

  };  // repl_t

}  // qmellow
