#pragma once

#include <algorithm>
#include <ostream>
#include <set>
#include <utility>
#include "match.h"

namespace qmellow {

/* The result of evaluating an expression. */
class result_t final {
  public:

  /* Convenience. */
  using matches_t = std::set<match_t>;

  /* Start out as a no-match result. */
  result_t() noexcept
      : success(false) {}

  /* The logical negation of the result. */
  result_t operator!() const {
    return result_t(!success, matches);
  }

  /* The logical-and of this result and that one. */
  result_t operator&&(const result_t& that) const {
    return (success == that.success)
        ? result_t(success && that.success, *this, that)
        : (!success ? *this : that);
  }

  /* The logical-or of this result and that one. */
  result_t operator||(const result_t& that) const {
    return (success == that.success)
        ? result_t(success || that.success, *this, that)
        : (success ? *this : that);
  }

  /* Add an individual match to the result.  This will make us a match, if
     we weren't already. */
  void add(match_t &&match) {
    success = true;
    matches.insert(std::move(match));
  }

  /* Write a human-readable description of the result, including whether this
     is a match or non-match and all the individual reasons for that
     evaluation. */
  void write(std::ostream &strm) const {
    strm << (success ? "match" : "no match") << std::endl;
    for (auto &match: matches) {
      match.write(strm);
      strm << std::endl;
    }
  }

  private:

  /* Used by the not-operator. */
  result_t(bool success, const matches_t &matches)
      : success(success), matches(matches) {}

  /* Used by the and- and or-operators. */
  result_t(bool success, const result_t &lhs, const result_t &rhs)
      : success(success) {
    /* Our matches will be the union of the sets of matches provided by
       the left- and right-hand sides. */
    std::set_union(
        lhs.matches.begin(), lhs.matches.end(),
        rhs.matches.begin(), rhs.matches.end(),
        std::inserter(matches, matches.begin()));
  }

  /* True iff. we're a match. */
  bool success;

  /* The individual reasons for our success or failure as a match. */
  matches_t matches;

};  // result_t

}  // qmellow
