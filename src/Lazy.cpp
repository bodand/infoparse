//
// Created by bodand on 2019-07-24.
//

#include "include.hpp"
#include INFO_PARSE_INCLUDE(Lazy.hpp)

#include <utility>

const char* info::parse::detail::bad_lazy_eval::what() const noexcept {
    static std::string ret = "Lazy<T> should have been evaluated for object type T, but provided initializer"
                             "for type T is not callable without parameters [with T = " + typname + "]";
    return ret.c_str();
}

info::parse::detail::bad_lazy_eval::bad_lazy_eval(std::string typname)
       : typname(std::move(typname)) {}
