#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include "file.h"
#include "match.h"
#include "result.h"

namespace qmellow {

/* The base of all kinds of expressions. */
class expr_t {
  public:

  /* Do-little. */
  virtual ~expr_t() {}

  /* Override to evaluate the expression on the given subject file. */
  virtual result_t eval(const file_t &file) const = 0;

  /* Override to pretty-print the expression as source script. */
  virtual void pretty_print(std::ostream &strm) const = 0;

  protected:

  /* Do-little. */
  expr_t() {}

};  // expr_t

/* The base an expression which owns no sub-expressions. */
class leaf_t
    : public expr_t, public match_t::cause_t {
  public:

  /* Satisfy our duty as a cause of a match */
  virtual void write(std::ostream &strm) const override final {
    pretty_print(strm);
  }

  protected:

  /* Do-little. */
  leaf_t() {}

};  // leaf_t

/* The base an expression which owns exactly one sub-expression. */
class affix_t
    : public expr_t {
  public:

  /* The sub-expression we own. */
  const expr_t *get_subexpr() const noexcept {
    return subexpr.get();
  }

  protected:

  /* Take ownership of the sub-expression. */
  affix_t(std::unique_ptr<expr_t> &&subexpr)
      : subexpr(std::move(subexpr)) {}

  private:

  /* See accessor. */
  std::unique_ptr<expr_t> subexpr;

};  // affix_t

/* The base an expression which owns exactly two sub-expressions. */
class infix_t
    : public expr_t {
  public:

  /* The left-hand sub-expression we own. */
  const expr_t *get_left_subexpr() const noexcept {
    return left_subexpr.get();
  }

  /* The right-hand sub-expression we own. */
  const expr_t *get_right_subexpr() const noexcept {
    return right_subexpr.get();
  }
  protected:

  /* Take ownership of the sub-expressions. */
  infix_t(
      std::unique_ptr<expr_t> &&left_subexpr,
      std::unique_ptr<expr_t> &&right_subexpr)
      : left_subexpr(std::move(left_subexpr)),
        right_subexpr(std::move(right_subexpr)) {}

  private:

  /* See accessors. */
  std::unique_ptr<expr_t> left_subexpr, right_subexpr;

};  // infix_t

/* ------------------------------------------------------------------------
   Expression final classes.
   ------------------------------------------------------------------------ */

/* Match CSS. */
class anchor_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  anchor_t(std::string &&text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_anchor(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << text;
  }

  private:

  /* The text to match.  It will start with a slash. */
  std::string text;

};  // anchor_t

/* Match a case-insensitive string. */
class case_insensitive_string_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  case_insensitive_string_t(const std::string &text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_case_insensitive_string(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << '\'' << text << '\'';
  }

  private:

  /* The text to match. */
  std::string text;

};  // case_insensitive_string_t

/* Match a case-sensitive string. */
class case_sensitive_string_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  case_sensitive_string_t(const std::string &text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_case_sensitive_string(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << '"' << text << '"';
  }

  private:

  /* The text to match. */
  std::string text;

};  // case_sensitive_string_t

/* Match class names. */
class class_names_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  class_names_t(std::vector<std::string> &&texts)
      : texts(std::move(texts)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_class_names(this, texts);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    for (auto &text: texts) {
      strm << '.' << text;
    }
  }

  private:

  /* The texts to match. */
  std::vector<std::string> texts;

};  // class_names_t

/* Match CSS. */
class css_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  css_t(std::string &&text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_css(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << text << ".css";
  }

  private:

  /* The text to match.  It will start with a slash. */
  std::string text;

};  // css_t

/* Match a CSS id. */
class css_id_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  css_id_t(const std::string &text)
      : text(text) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_css_id(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << "#" << text;
  }

  private:

  /* The text to match. */
  std::string text;

};  // css_id_t

/* Match an image. */
class image_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  image_t(std::string &&text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_image(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << text;
  }

  private:

  /* The text to match.  It will start with a slash. */
  std::string text;

};  // image_t

/* Match JS. */
class js_t final
    : public leaf_t {
  public:

  /* Cache the text to match. */
  js_t(std::string &&text)
      : text(std::move(text)) {}

  /* Evaluate the expression on the subject file. */
  virtual result_t eval(const file_t &file) const override {
    return file.match_js(this, text);
  }

  /* Pretty-print the expression. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << text << ".js";
  }

  private:

  /* The text to match.  It will start with a slash. */
  std::string text;

};  // js_t

/* Logical-not of a sub-expression. */
class not_t final
    : public affix_t {
  public:

  /* Take ownership of the sub-expression. */
  not_t(std::unique_ptr<expr_t> &&subexpr)
      : affix_t(std::move(subexpr)) {}

  /* Evaluate the sub-expression on the given file and negate the result. */
  virtual result_t eval(const file_t &file) const override {
    return !(get_subexpr()->eval(file));
  }

  /* Pretty-print the sub-expression to the given stream, putting a 'not'
     keyword in front of it. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << "not ";
    get_subexpr()->pretty_print(strm);
  }

};  // not_t

/* Grouping of a sub-expression inside parentheses. */
class group_t final
    : public affix_t {
  public:

  /* Take ownership of the sub-expression. */
  group_t(std::unique_ptr<expr_t> &&subexpr)
      : affix_t(std::move(subexpr)) {}

  /* Pass the evaluation through to the sub-expression. */
  virtual result_t eval(const file_t &file) const override {
    return get_subexpr()->eval(file);
  }

  /* Pretty-print the sub-expression to the given stream, putting parentheses
     around it. */
  virtual void pretty_print(std::ostream &strm) const override {
    strm << '(';
    get_subexpr()->pretty_print(strm);
    strm << ')';
  }

};  // not_t

/* Logical-and of two sub-expressions. */
class and_t final
    : public infix_t {
  public:

  /* Take ownership of the sub-expressions. */
  and_t(
      std::unique_ptr<expr_t> &&left_subexpr,
      std::unique_ptr<expr_t> &&right_subexpr)
      : infix_t(std::move(left_subexpr), std::move(right_subexpr)) {}

  /* Evaluate the sub-expressions on the given file and combine the
     results with an logical-and operation. */
  virtual result_t eval(const file_t &file) const override {
    return get_left_subexpr()->eval(file) && get_right_subexpr()->eval(file);
  }

  /* Pretty-print the sub-expressions to the given stream, putting an
     'and' keyword between them. */
  virtual void pretty_print(std::ostream &strm) const override {
    get_left_subexpr()->pretty_print(strm);
    strm << " and ";
    get_right_subexpr()->pretty_print(strm);
  }

};  // and_t

/* Logical-or of two sub-expressions. */
class or_t final
    : public infix_t {
  public:

  /* Take ownership of the sub-expressions. */
  or_t(
      std::unique_ptr<expr_t> &&left_subexpr,
      std::unique_ptr<expr_t> &&right_subexpr)
      : infix_t(std::move(left_subexpr), std::move(right_subexpr)) {}

  /* Evaluate the sub-expressions on the given file and combine the
     results with a logical-or operation. */
  virtual result_t eval(const file_t &file) const override {
    return get_left_subexpr()->eval(file) || get_right_subexpr()->eval(file);
  }

  /* Pretty-print the sub-expressions to the given stream, putting an
     'or' keyword between them. */
  virtual void pretty_print(std::ostream &strm) const override {
    get_left_subexpr()->pretty_print(strm);
    strm << " or ";
    get_right_subexpr()->pretty_print(strm);
  }

};  // or_t

}  // qmellow
