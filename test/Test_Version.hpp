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
#if defined(_WIN32) || defined(_WIN64)
    BOOST_CHECK(InfoParse::getVersion() == L"1.2");
#else
    BOOST_CHECK(InfoParse::getVersion() == "1.2");
#endif
}

#pragma clang diagnostic pop
