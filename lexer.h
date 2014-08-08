#pragma once

#include <cctype>
#include <map>
#include <vector>
#include "error.h"
#include "ice.h"
#include "pos.h"
#include "token.h"

namespace qmellow {

/* Convert source text into a vector of tokens. */
class lexer_t final {
  public:

  /* An error in lexing. */
  class error_t final
      : public qmellow::error_t {
    public:

    /* Report the position and what we found there. */
    error_t(const lexer_t *lexer, const char *msg)
        : qmellow::error_t(lexer->pos) {
      get_strm() << msg;
    }

  };  // lexer_t::error_t

  /* Convert the given source text into a vector of tokens, ending with an
     end token. */
  static std::vector<token_t> lex(const char *next_cursor) {
    return lexer_t(next_cursor).lex();
  }

  private:

  /* Used by our public lex function. */
  lexer_t(const char *next_cursor)
      : next_cursor(next_cursor), is_ready(false) {}

  /* Used by our public lex function. */
  std::vector<token_t> lex() {
    std::vector<token_t> tokens;
    enum { start, name, comment } state = start;
    const char *anchor = nullptr;
    pos_t anchor_pos;
    bool go = true;
    do {
      char c = peek();
      switch (state) {
        case start: {
          switch (c) {
            case '\0': {
              tokens.emplace_back(pos, token_t::end);
              go = false;
              break;
            }
            case '#': {
              tokens.emplace_back(pos, token_t::hash);
              pop();
              break;
            }
            case '.': {
              tokens.emplace_back(pos, token_t::dot);
              pop();
              break;
            }
            case '/': {
              tokens.emplace_back(pos, token_t::slash);
              pop();
              break;
            }
            case '(': {
              tokens.emplace_back(pos, token_t::open_paren);
              pop();
              break;
            }
            case ')': {
              tokens.emplace_back(pos, token_t::close_paren);
              pop();
              break;
            }
            case '\'': {
              tokens.emplace_back(
                  pos, token_t::single_string, lex_string());
              break;
            }
            case '"': {
              tokens.emplace_back(
                  pos, token_t::single_string, lex_string());
              break;
            }
            case '-': {
              pop();
              state = comment;
              break;
            }
            default: {
              if (isspace(c)) {
                pop();
                break;
              }
              if (isalpha(c) || c == '_') {
                anchor = cursor;
                anchor_pos = pos;
                pop();
                state = name;
                break;
              }
              throw error_t(this, "bad character");
            }
          }
          break;
        }
        case name: {
          if (isalnum(c) || c == '_') {
            pop();
          } else {
            if (!anchor) {
              throw ice_t(pos, __FILE__, __LINE__);
            }
            std::string text(anchor, cursor - anchor);
            anchor = nullptr;
            static const std::map<std::string, token_t::kind_t> keywords = {
              { "and", token_t::and_kwd },
              { "or", token_t::or_kwd },
              { "not", token_t::not_kwd }
            };
            auto iter = keywords.find(text);
            if (iter != keywords.end()) {
              tokens.emplace_back(anchor_pos, iter->second);
            } else {
              tokens.emplace_back(anchor_pos, token_t::name, std::move(text));
            }
            state = start;
          }
          break;
        }
        case comment: {
          pop();
          if (!c || c == '\n') {
            state = start;
          }
          break;
        }
      }  // switch
    } while (go);
    return std::move(tokens);
  }

  /* Lex a string starting and ending with a quote mark of some kind and
     possibly containing escape characters.  The returned string will not
     have the quotes around it and will have the escapes translated. */
  std::string lex_string() {
    std::ostringstream strm;
    char quote = pop();
    enum state { normal, escaped } state = normal;
    bool go = true;
    do {
      char c = peek();
      if (!c) {
        throw error_t(this, "end-of-program inside quoted string");
      }
      switch (state) {
        case normal: {
          if (c == '\\') {
            pop();
            state = escaped;
            break;
          }
          if (c == quote) {
            pop();
            go = false;
            break;
          }
          if (c >= ' ' && c <= 'z') {
            strm.put(c);
            pop();
            break;
          }
          throw error_t(this, "bad character in quoted string");
        }
        case escaped: {
          switch (c) {
            case '\\':
            case '\'':
            case '"': {
              strm.put(c);
              pop();
              break;
            }
            case 'n': {
              strm.put('\n');
              pop();
            }
            case 'r': {
              strm.put('\t');
              pop();
            }
            case 't': {
              strm.put('\t');
              pop();
            }
            default: {
              throw error_t(this, "bad escape character in quoted string");
            }
          }  // switch
          state = normal;
          break;
        }
      }  // switch
    }  while (go);
    return strm.str();
  }

  /* Return the current character from the source text but don't advance to
     the next one. */
  char peek() const {
    if (!is_ready) {
      cursor = next_cursor;
      pos = next_pos;
      switch (*cursor) {
        case '\0': {
          break;
        }
        case '\n': {
          ++next_cursor;
          next_pos.next_line();
          break;
        }
        default: {
          ++next_cursor;
          next_pos.next_col();
        }
      }  // switch
      is_ready = true;
    }
    return *cursor;
  }

  /* Return the current character from the source text and advance to the
     next one. */
  char pop() {
    char c = peek();
    is_ready = false;
    return c;
  }

  /* Our next position within the source text. */
  mutable const char *next_cursor;

  /* The (line, col) of next_cursor. */
  mutable pos_t next_pos;

  /* If true, then cursor and pos, below, are valid; otherwise,
     those fields contain junk.  Peeking makes us ready, popping makes us
     unready. */
  mutable bool is_ready;

  /* Our current position within the source text, when ready. */
  mutable const char *cursor;

  /* The (line, col) cursor, when ready. */
  mutable pos_t pos;

};  // lexer_t

}  // qmellow
