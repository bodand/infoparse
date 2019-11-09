//
// Created by bodand on 2019-11-09.
//

#pragma once

#include <boost/preprocessor/stringize.hpp>

#define INFO_PARSE_NEO_INCLUDE(Header) \
    BOOST_PP_STRINGIZE(../../include/info/parse/neo/Header)
