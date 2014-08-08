#pragma once

#include <ostream>

namespace qmellow {

/* An individual match in a subject file. */
class match_t final {
  public:

  /* The cause of a match. */
  class cause_t {
    public:

    /* Override to describe the cause. */
    virtual void write(std::ostream &strm) const = 0;

    protected:

    /* Do-little. */
    cause_t() {}

    /* Do-little. */
    virtual ~cause_t() {}

  };  // match_t::cause_t;

  /* Cache the arguments. */
  explicit match_t(int line_number, const cause_t *cause)
      : line_number(line_number), cause(cause) {}

  /* Strict weak ordering by line number, then by cause. */
  bool operator<(const match_t &that) const {
    return line_number < that.line_number
        || (line_number == that.line_number && cause < that.cause);
  }

  /* Write a human-readable description of the match, giving the line number
     and the cause-expression which matched. */
  void write(std::ostream &strm) const {
    strm << line_number << ", [";
    cause->write(strm);
    strm << ']';
  }

  private:

  /* The line number of the subject file on which the match occurred. */
  int line_number;

  /* The cause of this matched. */
  const cause_t *cause;

};  // match_t

}  // qmellow
