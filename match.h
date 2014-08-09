#pragma once

#include <string>

namespace qmellow {

/* An individual match in a subject file. */
class match_t final {
  public:

  /* The cause of a match. */
  class cause_t {
    public:

    /* Override to describe the cause. */
    virtual const std::string &get_desc() const = 0;

    protected:

    /* Do-little. */
    cause_t() {}

    /* Do-little. */
    virtual ~cause_t() {}

  };  // match_t::cause_t;

  /* Cache the arguments. */
  explicit match_t(
        const cause_t *cause, const std::string &sub_file_path,
        int line_number, const std::string &line_text)
      : cause(cause), sub_file_path(sub_file_path),
        line_number(line_number), line_text(line_text) {}

  /* Cache the arguments. */
  explicit match_t(
        const cause_t *cause,
        int line_number, const std::string &line_text)
      : cause(cause), line_number(line_number), line_text(line_text) {}

  /* Strict weak ordering by line number, then by cause. */
  bool operator<(const match_t &that) const {
    return line_number < that.line_number
        || (line_number == that.line_number && cause < that.cause);
  }

  /* A string describing the cause of the match. */
  const std::string &get_cause_desc() const {
    return cause->get_desc();
  }

  /* The number of the line in the subject (or sub-file) on which the match
     occurred. */
  int get_line_number() const noexcept {
    return line_number;
  }

  /* The text of the line in the subject (or sub-file) on which the match
     occurred. */
  const std::string &get_line_text() const noexcept {
    return line_text;
  }

  /* The sub-file (such as server-side include, a CSS, or JS file) in which
     the match occurred.  If the match occurred in the subject file, this
     string will be empty. */
  const std::string &get_sub_file_path() const noexcept {
    return sub_file_path;
  }

  private:

  /* The cause of this match. */
  const cause_t *cause;

  /* See accessor. */
  std::string sub_file_path;

  /* See accessor. */
  int line_number;

  /* See accessor. */
  std::string line_text;

};  // match_t

}  // qmellow
