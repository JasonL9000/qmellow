#pragma once

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "error.h"
#include "expr.h"
#include "ice.h"
#include "token.h"
#include "utils.h"

namespace qmellow {

/* Convert an array of tokens into a syntax tree. */
class parser_t final {
  public:

  /* Thrown by match_token, below, when it doesn't find the right kind of
     token. */
  class error_t final
      : public qmellow::error_t {
    public:

    /* Cache a pointer to the token which gave rise to the error and the
       set of kinds we were expecting to find.  The token will not be among
       these kinds. */
    error_t(
        const parser_t *parser, const std::set<token_t::kind_t> &kinds)
        : qmellow::error_t(parser->cursor->get_pos()) {
      const char *before_desc = "expected ";
      for (auto kind: kinds) {
        get_strm() << before_desc << token_t::get_desc(kind);
        before_desc = " or ";
      }
      end_section();
      get_strm()
          << "found " << token_t::get_desc(parser->cursor->get_kind());
    }

  };  // parser_t::error_t

  /* Convert an array of tokens into a syntax tree. */
  static std::unique_ptr<expr_t> parse(const token_t *cursor) {
    return parser_t(cursor).parse();
  }

  private:

  /* Used by our public parse function. */
  parser_t(const token_t *cursor)
      : cursor(cursor) {}

  /* Like try_match_token, below, but, if we don't match, we throw a syntax
     error.  This version never returns a null pointer. */
  const token_t *match_token(const std::set<token_t::kind_t> &kinds) {
    auto *token = try_match_token(kinds);
    if (!token) {
      throw error_t(this, kinds);
    }
    return token;
  }

  /* Parse a whole program and expect to find the end token. */
  std::unique_ptr<expr_t> parse() {
    auto expr = parse_ors();
    match_token({ token_t::end });
    return std::move(expr);
  }

  /* Parse a series of and-operations. */
  std::unique_ptr<expr_t> parse_ands() {
    std::unique_ptr<expr_t> expr = parse_nots();
    while (try_match_token({ token_t::and_kwd })) {
      expr = make_unique<and_t>(std::move(expr), parse_nots());
    }
    return std::move(expr);
  }

  /* Parse a leaf or a group. */
  std::unique_ptr<expr_t> parse_atom() {
    std::unique_ptr<expr_t> expr;
    auto *token = match_token({
        token_t::single_string, token_t::double_string, token_t::hash,
        token_t::dot, token_t::slash, token_t::open_paren });
    switch (token->get_kind()) {
      case token_t::single_string: {
        expr = make_unique<case_insensitive_string_t>(token->get_text());
        break;
      }
      case token_t::double_string: {
        expr = make_unique<case_sensitive_string_t>(token->get_text());
        break;
      }
      case token_t::hash: {
        token = match_token({ token_t::name });
        expr = make_unique<css_id_t>(token->get_text());
        break;
      }
      case token_t::dot: {
        expr = make_unique<class_names_t>(parse_dotted_names());
        break;
      }
      case token_t::slash: {
        std::ostringstream strm;
        for (;;) {
          strm << '/';
          auto texts = parse_dotted_names();
          if (!try_match_token({ token_t::slash })) {
            if (texts.back() == "css") {
              texts.pop_back();
              write_dotted_names(strm, texts);
              expr = make_unique<css_t>(strm.str());
            } else if (texts.back() == "js") {
              texts.pop_back();
              write_dotted_names(strm, texts);
              expr = make_unique<js_t>(strm.str());
            } else if (texts.back() == "png") {
              write_dotted_names(strm, texts);
              expr = make_unique<image_t>(strm.str());
            } else if (texts.back() == "jpg") {
              write_dotted_names(strm, texts);
              expr = make_unique<image_t>(strm.str());
            } else if (texts.back() == "svg") {
              write_dotted_names(strm, texts);
              expr = make_unique<image_t>(strm.str());
            } else if (texts.back() == "gif") {
              write_dotted_names(strm, texts);
              expr = make_unique<image_t>(strm.str());
            } else {
              write_dotted_names(strm, texts);
              expr = make_unique<anchor_t>(strm.str());
            }
            break;
          }
          write_dotted_names(strm, texts);
        }  // for
        break;
      }
      case token_t::open_paren: {
        expr = make_unique<group_t>(parse_ors());
        match_token({ token_t::close_paren });
        break;
      }
      default:
        throw ice_t(token->get_pos(), __FILE__, __LINE__);
    }  // switch
    return std::move(expr);
  }

  /* Parse something that looks like "a.b.c" and return it as a vector of
     individual texts. */
  std::vector<std::string> parse_dotted_names() {
    std::vector<std::string> texts;
    do {
      texts.push_back(match_token({ token_t::name })->get_text());
    } while (try_match_token({ token_t::dot }));
    return std::move(texts);
  }

  /* Parse some number of not-operations, followed by an atom. */
  std::unique_ptr<expr_t> parse_nots() {
    bool is_not = false;
    while (try_match_token({ token_t::not_kwd })) {
      is_not = !is_not;
    }
    std::unique_ptr<expr_t> expr = parse_atom();
    if (is_not) {
      expr = make_unique<not_t>(std::move(expr));
    }
    return std::move(expr);
  }

  /* Parse a series of or-operations. */
  std::unique_ptr<expr_t> parse_ors() {
    std::unique_ptr<expr_t> expr = parse_ands();
    while (try_match_token({ token_t::or_kwd })) {
      expr = make_unique<or_t>(std::move(expr), parse_ands());
    }
    return std::move(expr);
  }

  /* If the token pointed to by cursor is of one of the expected kinds,
     return the token and advance the cursor; otherwise, leave the cursor
     where it is and return a null pointer. */
  const token_t *try_match_token(const std::set<token_t::kind_t> &kinds) {
    auto iter = kinds.find(cursor->get_kind());
    return (iter != kinds.end()) ? cursor++ : nullptr;
  }

  /* Reverse the parse_dotted_names function, above, writing out the texts as
     a single string. */
  static void write_dotted_names(
      std::ostream &strm, const std::vector<std::string> &texts) {
    const char *before_text = "";
    for (const auto &text: texts) {
      strm << before_text << text;
      before_text = ".";
    }
  }

  /* Our current position in the array of tokens. */
  const token_t *cursor;

};  // parser_t

}  // qmellow
