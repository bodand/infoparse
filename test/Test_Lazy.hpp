//
// Created by bodand on 2019-07-25.
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>
#include "../include/info/parse/Lazy.hpp"

using namespace info::parse::detail;

BOOST_AUTO_TEST_SUITE(Test_Lazy)

  BOOST_AUTO_TEST_CASE(Test_Lazy_InitiallyNotInited) {
      Lazy<int> l([]() { return std::make_shared<int>(5); });
      BOOST_CHECK(!l.isInited());
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_InitedAfterGet) {
      Lazy<int> l([]() { return std::make_shared<int>(5); });
      int i = l.get();
      BOOST_CHECK(l.isInited());
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_IniterIsOnlyCalledOnce) {
      int count = 0;
      Lazy<int> l([&]() {
        count++;
        return std::make_shared<int>(5);
      });
      BOOST_REQUIRE_EQUAL(count, 0);
      int i = l.get();
      BOOST_REQUIRE_EQUAL(count, 1);
      int j = l.get();
      BOOST_CHECK_EQUAL(count, 1);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_CorrectValueIsGotten) {
      int val = 5;
      Lazy<int> l([=]() { return std::make_shared<int>(val); });
      int i = l.get();
      BOOST_CHECK_EQUAL(val, i);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_CastingToIntWorks) {
      int val = 5;
      Lazy<int> l([=]() { return std::make_shared<int>(val); });
      int i = l;
      BOOST_CHECK_EQUAL(val, i);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_CastingToIntDoesNotCauseMultipleCallsToIniter) {
      int count = 0;
      Lazy<int> l([&]() {
        count++;
        return std::make_shared<int>(5);
      });
      BOOST_REQUIRE_EQUAL(count, 0);
      int i = l;
      BOOST_REQUIRE_EQUAL(count, 1);
      int j = l;
      BOOST_CHECK_EQUAL(count, 1);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_DereferenceOperatorEqualsGetMethod) {
      Lazy<int> l([]() { return std::make_shared<int>(5); });
      BOOST_CHECK_EQUAL(*l, 5);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_ParensOperatorInstantiatesTWithParamRequirements) {
      Lazy<int, int> l([](int i) { return std::make_shared<int>(i); });
      BOOST_CHECK_EQUAL(l(4), 4);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_AfterInitedDerefernceOperatorWorksWithArgumentRequiringT) {
      Lazy<int, int> l([](int i) { return std::make_shared<int>(i); });
      BOOST_REQUIRE_EQUAL(l(4), 4);
      BOOST_CHECK_EQUAL(*l, 4);
  }

  struct Foo {
      Foo(int i)
              : i(i) {}
      int i;
  };

  BOOST_AUTO_TEST_CASE(Test_Lazy_AfterInitedArrowDerefernceOperatorWorksWithArgumentRequiringT) {
      Lazy<Foo, int> l([](int i) { return std::make_shared<Foo>(i); });
      BOOST_REQUIRE_EQUAL(l(4).i, 4);
      BOOST_CHECK_EQUAL(l->i, 4);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_DereferenceOpartorThrowExceptionWhenTRequiresArgs) {
      Lazy<int, int> l([](int i) { return std::make_shared<int>(i); });
      BOOST_CHECK_THROW(auto i = *l, bad_lazy_eval);
  }

  BOOST_AUTO_TEST_CASE(Test_Lazy_ArrowDereferenceOpartorThrowExceptionWhenTRequiresArgs) {
      Lazy<Foo, int> l([](int i) { return std::make_shared<Foo>(i); });
      BOOST_CHECK_THROW(auto i = l->i, bad_lazy_eval);
  }

BOOST_AUTO_TEST_SUITE_END()
