#pragma once

#include <ostream>
#include <string>
#include <utility>
#include "pos.h"

namespace qmellow {

/* A single token from the source text.  It consists of a kind, describing
   the general variety of token it is, and a text which is copied from the
   source which gave rise to this token. */
class token_t final {
  public:

  /* Tokens come in various kinds. */
  enum kind_t {
    end, hash, dot, slash, name,
    open_paren, close_paren,
    single_string, double_string,
    and_kwd, or_kwd, not_kwd
  };

  /* Cache the position and kind and set the text to the empty string. */
  token_t(const pos_t &pos, kind_t kind)
      : pos(pos), kind(kind) {}

  /* Cache the position and kind and the text. */
  token_t(const pos_t &pos, kind_t kind, std::string &&text)
      : pos(pos), kind(kind), text(std::move(text)) {}

  /* See kind_t, above. */
  kind_t get_kind() const noexcept {
    return kind;
  }

  /* The position in the source at which this token appeared. */
  const pos_t &get_pos() const noexcept {
    return pos;
  }

  /* The text that gave rise to this token, if relevant; otherwise, this is
     the empty string. */
  const std::string &get_text() const noexcept {
    return text;
  }

  /* A human-readable description of a token kind. */
  static const char *get_desc(kind_t kind) {
    const char *desc;
    switch (kind) {
      case end: desc = "end-of-program"; break;
      case hash: desc = "'#'"; break;
      case dot: desc = "'.'"; break;
      case slash: desc = "'/'"; break;
      case name: desc = "a name"; break;
      case open_paren: desc = "'('"; break;
      case close_paren: desc = "')'"; break;
      case single_string: desc = "a single-quoted string"; break;
      case double_string: desc = "a double-quoted string"; break;
      case and_kwd: desc = "'and'"; break;
      case or_kwd: desc = "'or'"; break;
      case not_kwd: desc = "'not'"; break;
    }
    return desc;
  }

  /* Writes a human-readable dump of the token.  This is for debugging
     purposes only.  In production, a user never sees tokens directly. */
  friend std::ostream &operator<<(std::ostream &strm, const token_t &that) {
    strm << that.pos << "; " << get_desc(that.kind);
    if (!that.text.empty()) {
      strm << "; \"" << that.text << '"';
    }
    return strm;
  }

  private:

  /* See accessors. */
  pos_t pos;

  /* See accessor. */
  kind_t kind;

  /* See accessor. */
  std::string text;

};  // token_t

}  // qmellow
