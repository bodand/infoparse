/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-02-05.
//


#include "Option_.hpp"

info::parse::detail::bad_function_callback::bad_function_callback(int a)
        : logic_error("Too many parameters required for function callback maximum is 2. [with sizeof...(Args) = "
                      + std::to_string(a) +
                      "] ") {}
