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

BOOST_AUTO_TEST_SUITE(Trivial)

  /**
   * I always have some trivial passing test to test the setup
   */
  BOOST_AUTO_TEST_CASE(Test_test) {
      BOOST_CHECK_MESSAGE(true, "Your Boost.Test setup is working");
  }

BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
