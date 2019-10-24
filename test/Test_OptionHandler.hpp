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

#define CREATE_HANDLER \
    auto* handler = new OptionHandler_<decltype(data)>

#include "../src/OptionHandler_.hpp"
#include "../src/Option_.hpp"

using namespace info::parse;
using namespace info::parse::detail;

BOOST_AUTO_TEST_SUITE(Test_OptionHandler)

  BOOST_AUTO_TEST_CASE(Test_OptionHandler_OptionHandlerCanBeInstantiated) {
      auto* handler = new OptionHandler_<int>;
      BOOST_CHECK_NE(handler, nullptr);
      delete handler;
  }

  BOOST_AUTO_TEST_CASE(Test_OptionHandler_AdditionOfAlreadyExistingOptions) {
      int data;
      Option_<int> opt("name", &data);
      CREATE_HANDLER;
      handler->addOption(std::move(opt));
      auto& opts = handler->options();
      BOOST_CHECK_NE(opts.size(), 0);
      auto i = opts.begin();
      while (*i != "Option<i>[name]" && i != opts.end()) {
          ++i;
      }
      BOOST_CHECK(i != opts.end());
      delete handler;
  }

  BOOST_AUTO_TEST_CASE(Test_OptionHandler_AdditionOfConstructedOptionWithDeducedShortName) {
      int data;
      CREATE_HANDLER;
      handler->addOption("name", &data);
      auto& opts = handler->options();
      BOOST_CHECK_NE(opts.size(), 0);
      auto i = opts.begin();
      while (*i != "name" && i != opts.end()) {
          ++i;
      }
      BOOST_CHECK(i != opts.end());
      delete handler;
  }

  BOOST_AUTO_TEST_CASE(Test_OptionHandler_AdditionOfConstructedOptionWithExplicitShortName) {
      int data;
      CREATE_HANDLER;
      handler->addOption("name|p", &data);
      auto& opts = handler->options();
      BOOST_CHECK_NE(opts.size(), 0);
      auto i = opts.begin();
      while (*i != "name" && i != opts.end()) {
          ++i;
      }
      BOOST_CHECK(i != opts.end());
      delete handler;
  }

  BOOST_AUTO_TEST_CASE(Test_OptionHandler_RemovalOfAddedOption) {
      int data;
      Option_<int> opt("name", &data);
      CREATE_HANDLER;
      handler->addOption(std::move(opt));
      auto& opts = handler->options();
      handler->rmOption("name");
      BOOST_CHECK_EQUAL(handler->options().size(), 0);
      delete handler;
  }

BOOST_AUTO_TEST_SUITE_END()
#pragma clang diagnostic pop
