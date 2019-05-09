//
// Created by tudom on 2019. 05. 01..
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#define CREATE_HANDLER \
    auto* handler = new _OptionHandler<decltype(data)>

#include "../_OptionHandler.hpp"
#include "../_Option.hpp"

using namespace InfoParse;

BOOST_AUTO_TEST_CASE(Test_OptionHandler_OptionHandlerCanBeInstantiated) {
    auto* handler = new _OptionHandler<int>;
    BOOST_CHECK_NE(handler, nullptr);
    delete handler;
}

BOOST_AUTO_TEST_CASE(Test_OptionHandler_AdditionOfAlreadyExistingOptions) {
    int data;
    _Option<int> opt("name", &data);
    CREATE_HANDLER;
    handler->addOption(std::move(opt));
    auto& opts = handler->getOptions();
    BOOST_CHECK_NE(opts.size(), 0);
    auto i = opts.begin();
    while (*i != "name" && i != opts.end()) {
        ++i;
    }
    BOOST_CHECK(i != opts.end());
    delete handler;
}

BOOST_AUTO_TEST_CASE(Test_OptionHandler_AdditionOfConstructedOptionWithDeducedShortName) {
    int data;
    CREATE_HANDLER;
    handler->addOption("name", &data);
    auto& opts = handler->getOptions();
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
    handler->addOption("name", 'p', &data);
    auto& opts = handler->getOptions();
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
    _Option<int> opt("name", &data);
    CREATE_HANDLER;
    handler->addOption(std::move(opt));
    auto& opts = handler->getOptions();
    handler->rmOption("name");
    BOOST_CHECK_EQUAL(handler->getOptions().size(), 0);
    delete handler;
}

#pragma clang diagnostic pop
