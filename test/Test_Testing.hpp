//
// Created by tudom on 2019. 05. 01..
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>

// I always have some trivial passing test to test the setup
BOOST_AUTO_TEST_CASE(Test_test) {
    BOOST_CHECK_EQUAL(1, 1);
}

#pragma clang diagnostic pop
