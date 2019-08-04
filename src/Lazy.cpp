//
// Created by bodand on 2019-07-24.
//

#include "Lazy.hpp"

#include <utility>

const char* InfoParse::Internals::bad_lazy_eval::what() const noexcept {
    return ("Lazy<T> should have been evaluated for object type T, but provided initializer"
            "for type T is not callable without parameters [with T = " + typname + "]").c_str();
}

InfoParse::Internals::bad_lazy_eval::bad_lazy_eval(std::string typname)
        : typname(std::move(typname)) {}