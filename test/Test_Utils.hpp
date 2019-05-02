//
// Created by tudom on 2019. 05. 02..
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <boost/test/included/unit_test.hpp>
#include "../utils.hpp"

#include <vector>
#include <string>

using namespace InfoParse;

BOOST_AUTO_TEST_CASE(Test_Utils_MonolithArgsWithoutInnerSpace) {
    int _argc = 3;
    char* _argv[]{const_cast<char*>("abc"),
                  const_cast<char*>("asd"),
                  const_cast<char*>("def")};
    BOOST_CHECK_EQUAL(makeMonolithArgs(_argc, _argv), " abc asd def ");
}

BOOST_AUTO_TEST_CASE(Test_Utils_MonolithArgsWithInnerSpace) {
    int _argc = 3;
    char* _argv[]{const_cast<char*>("abc cba"),
                  const_cast<char*>("asd     dsa"),
                  const_cast<char*>("def$fed")};
    BOOST_CHECK_EQUAL(makeMonolithArgs(_argc, _argv), " abc$1$cba asd$5$dsa def\\$fed ");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrDoesNotContainSpaces) {
    std::string spacey("asd asd      asd2");
    itrStr(spacey);
    BOOST_CHECK_EQUAL(spacey.find(' '), std::string::npos);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrEscapes$Correctly) {
    std::string spacey("$asd asd      asd2");
    itrStr(spacey);
    BOOST_CHECK_EQUAL(spacey.substr(0, 2), "\\$");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrCountsSpacesCorrectly) {
    std::string spacey("asd      asd2");
    itrStr(spacey);
    BOOST_CHECK_EQUAL(spacey, "asd$6$asd2");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ArcItrStrIsInverseOfItrStr) {
    std::string spacey("asd asd      asd2");
    std::string spacey_2("asd asd      asd2");
    itrStr(spacey);
    arcItrStr(spacey);
    BOOST_CHECK_EQUAL(spacey, spacey_2);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrDoesNotContainSpacesW) {
    std::wstring spacey(L"asd asd      asd2");
    itrStr(spacey);
    BOOST_CHECK_EQUAL(spacey.find(L' '), std::wstring::npos);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrEscapes$CorrectlyW) {
    std::wstring spacey(L"$asd asd      asd2");
    itrStr(spacey);
    BOOST_CHECK(spacey.substr(0, 2) == L"\\$");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ItrStrCountsSpacesCorrectlyW) {
    std::wstring spacey(L"asd      asd2");
    itrStr(spacey);
    BOOST_CHECK(spacey == L"asd$6$asd2");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ArcItrStrIsInverseOfItrStrW) {
    std::wstring spacey(L"asd asd      asd2");
    std::wstring spacey_2(L"asd asd      asd2");
    itrStr(spacey);
    arcItrStr(spacey);
    BOOST_CHECK(spacey == spacey_2);
}

#pragma clang diagnostic pop

