//
// Created by bodand on 2019-07-25.
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>

#include "../src/OptionString.hpp"

BOOST_AUTO_TEST_SUITE(Test_OptionString)
  using namespace info::parse;

  BOOST_AUTO_TEST_CASE(Test_OptionString_SplitsStringCorrectly) {
      OptionString os("apple|a|ap");
      std::vector<std::string> ex{"-apple", "-a", "-ap"};
      BOOST_REQUIRE_NE(&os, nullptr);
      auto& names = os.getNames();
      BOOST_CHECK_EQUAL_COLLECTIONS(
        names.begin(), names.end(),
        ex.begin(), ex.end()
      );
  }

  BOOST_AUTO_TEST_CASE(Test_OptionString_HasShortReturnCorrectlyTrue) {
      OptionString os("apple|a|ap");
      std::vector<std::string> ex{"-apple", "-a", "-ap"};
      BOOST_REQUIRE_NE(&os, nullptr);
      auto& names = os.getNames();
      BOOST_REQUIRE_EQUAL_COLLECTIONS(
              names.begin(), names.end(),
              ex.begin(), ex.end()
      );
      BOOST_CHECK(os.hasShort());
  }

  BOOST_AUTO_TEST_CASE(Test_OptionString_HasShortReturnCorrectlyFalse) {
      OptionString os("apple|ap");
      std::vector<std::string> ex{"-apple", "-ap"};
      BOOST_REQUIRE_NE(&os, nullptr);
      auto& names = os.getNames();
      BOOST_REQUIRE_EQUAL_COLLECTIONS(
              names.begin(), names.end(),
              ex.begin(), ex.end()
      );
      BOOST_CHECK(!os.hasShort());
  }

BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
