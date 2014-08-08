#pragma once

#include <string>
#include <vector>
#include "match.h"
#include "result.h"

namespace qmellow {

/* A file to match against. */
class file_t {
  public:

  /* Borrow this type. */
  using cause_t = match_t::cause_t;

  /* TODO */
  result_t match_anchor(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_case_insensitive_string(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_case_sensitive_string(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_class_names(
        const cause_t *cause, const std::vector<std::string> &texts) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_css(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_css_id(const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

  /* TODO */
  result_t match_js(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    return std::move(result);
  }

};  // file_t

}  // qmellow
