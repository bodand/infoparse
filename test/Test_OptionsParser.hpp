/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019. 05. 09..
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>

#include "../OptionsParser.hpp"

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionParserCanBeInstantiated) {
    auto* parser = new OptionsParser;
    BOOST_CHECK_NE(parser, nullptr);
    delete parser;
}

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionParserParsesLongFlags) {
    auto* parser = new OptionsParser;
    BOOST_REQUIRE_NE(parser, nullptr);
    bool a, b, c;
    parser->addOption("alpha", &a);
    parser->addOption("beta", &b);
    parser->addOption("gamma", &c);
    parser->parse(" --alpha --gamma ");
    BOOST_CHECK(a);
    BOOST_CHECK(!b);
    BOOST_CHECK(c);
    delete parser;
}

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionParserParsesLongOptions) {
    auto* parser = new OptionsParser;
    BOOST_REQUIRE_NE(parser, nullptr);
    int a = 0, b = 0, c = 0;
    parser->addOption("alpha", &a);
    parser->addOption("beta", &b);
    parser->addOption("gamma", &c);
    parser->parse(" --alpha 42 --gamma 123 ");
    BOOST_CHECK_EQUAL(a, 42);
    BOOST_CHECK_EQUAL(b, 0);
    BOOST_CHECK_EQUAL(c, 123);
    delete parser;
}

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionsParserParsesShortFlags) {
    auto* parser = new OptionsParser;
    BOOST_REQUIRE_NE(parser, nullptr);
    bool a, b, c;
    parser->addOption("alpha", &a);
    parser->addOption("beta", &b);
    parser->addOption("gamma", &c);
    parser->parse(" -a -g ");
    BOOST_CHECK(a);
    BOOST_CHECK(!b);
    BOOST_CHECK(c);
    delete parser;
}

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionParserParsesShortOptions) {
    auto* parser = new OptionsParser;
    BOOST_REQUIRE_NE(parser, nullptr);
    int a = 0, b = 0, c = 0;
    parser->addOption("alpha", &a);
    parser->addOption("beta", &b);
    parser->addOption("gamma", &c);
    parser->parse(" -a 42 -g 123 ");
    BOOST_CHECK_EQUAL(a, 42);
    BOOST_CHECK_EQUAL(b, 0);
    BOOST_CHECK_EQUAL(c, 123);
    delete parser;
}

BOOST_AUTO_TEST_CASE(Test_OptionsParser_OptionParserParsesBundledArguments) {
    auto* parser = new OptionsParser;
    BOOST_REQUIRE_NE(parser, nullptr);
    int a = 0, b = 0, c = 0;
    bool aa, bb, cc;
    parser->addOption("alpha", &a);
    parser->addOption("beta", &b);
    parser->addOption("gamma", &c);
    parser->addOption("die", &aa);
    parser->addOption("sleep", &bb);
    parser->addOption("observe", &cc);
    parser->parse(" -dsa 42 -g 123 ");
    BOOST_CHECK_EQUAL(a, 42);
    BOOST_CHECK_EQUAL(b, 0);
    BOOST_CHECK_EQUAL(c, 123);
    BOOST_CHECK(aa);
    BOOST_CHECK(bb);
    BOOST_CHECK(!cc);
    delete parser;
}

#pragma clang diagnostic pop
