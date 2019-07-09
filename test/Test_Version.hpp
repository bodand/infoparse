/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by tudom on 2019. 05. 01..
//
#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>

#include "../versioning.hpp"

BOOST_AUTO_TEST_CASE(Test_Version) {
    BOOST_CHECK(InfoParse::getVersion() == "1.2");
}

#pragma clang diagnostic pop
