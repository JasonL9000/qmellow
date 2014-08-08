#pragma once

#include <fstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <sstream>
#include <memory>
#include <utility>

namespace qmellow {

/* For now, we'll borrow this function from the future C++ standard.  When we
   switch to the '14 version of the compiler, we'll just use
   std::make_unique. */
template<typename obj_t, typename... args_t>
inline std::unique_ptr<obj_t> make_unique(args_t &&... args) {
    return std::unique_ptr<obj_t>(new obj_t(std::forward<args_t>(args)...));
}

/* Read a while file into a string. */
std::string read_whole_file(const std::string &path) {
  try {
    std::ifstream strm(path);
    strm.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    strm.seekg(0, std::ios::end);
    std::string text;
    text.reserve(strm.tellg());
    strm.seekg(0, std::ios::beg);
    text.assign(
        std::istreambuf_iterator<char>(strm),
        std::istreambuf_iterator<char>());
    return std::move(text);

  } catch (...) {
    std::ostringstream strm;
    strm << "could not read from \"" << path << '"';
    throw std::runtime_error(strm.str());
  }
}

}  // qmellow
