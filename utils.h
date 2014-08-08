#pragma once

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

}  // qmellow
