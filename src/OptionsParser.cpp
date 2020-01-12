/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-02-06.
//

#include "include.hpp"
#include INFO_PARSE_INCLUDE(OptionsParser.hpp)

info::parse::detail::OptionAdder::OptionAdder(OptionsParser* parser)
        : _mother(parser) {}



