#pragma once

#include <ostream>

namespace qmellow {

/* A line-and-column position within a source text.  We use this when
   reporting errors, so we know where the error arose. */
class pos_t final {
  public:

  /* Start at line 1, column 1. */
  pos_t() noexcept
      : line_number(1), col_number(1) {}

  /* Advance to the next column. */
  void next_col() {
    ++col_number;
  }

  /* Advance to the first column of the next line. */
  void next_line() {
    ++line_number;
    col_number = 1;
  }

  /* Write a human-readable version. */
  friend std::ostream &operator<<(std::ostream &strm, const pos_t &that) {
    return strm
        << "line " << that.line_number
        << ", col " << that.col_number;
  }

  private:

  /* Our line and column numbers. */
  int line_number, col_number;

};  // pos_t

}  // qmellow
