#pragma once

#include <set>
#include <string>
#include <vector>
#include "match.h"
#include "result.h"
#include "utils.h"

namespace qmellow {

/* A file to match against. */
class file_t {
  public:

  /* Borrow this type. */
  using cause_t = match_t::cause_t;

  /* TODO: construct from some JS thing. */
  file_t() {}

  /* Find matching anchors. */
  result_t match_anchor(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching strings without regard to case. */
  result_t match_case_insensitive_string(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching strings. */
  result_t match_case_sensitive_string(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching class names (within a single element). */
  result_t match_class_names(
        const cause_t *cause, const std::vector<std::string> &texts) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching CSS includes. */
  result_t match_css(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching CSS ids. */
  result_t match_css_id(const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching images. */
  result_t match_image(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  /* Find matching JS includes. */
  result_t match_js(
        const cause_t *cause, const std::string &text) const {
    result_t result;
    /* TODO: call result.add(match_t(...)) for each match. */
    return std::move(result);
  }

  private:

  /* TODO: add state you'll use in the match functions, above. */

};  // file_t

}  // qmellow
