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

BOOST_AUTO_TEST_CASE(Test_Utils_StringWithExactSpaceMarkerformatIsEscapedCorrectly) {
    std::string dollary("asd asd$4$asd2");
    itrStr(dollary);
    BOOST_CHECK_EQUAL(dollary, R"(asd$1$asd\$4\$asd2)");
}

BOOST_AUTO_TEST_CASE(Test_Utils_StringWithExactSpaceMarkerformatIsArcosedCorrectly) {
    std::string dollary("asd asd$4$asd2");
    itrStr(dollary);
    arcItrStr(dollary);
    BOOST_CHECK_EQUAL(dollary, "asd asd$4$asd2");
}

class Base {
};

class Sub : public Base {
};

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_extends) {
    bool ret = Internals::extends<Sub, Base>();
    BOOST_CHECK(ret);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_extends_false) {
    bool ret = Internals::extends<Base, Sub>();
    BOOST_CHECK(!ret);
}

class BaseFactory {
public:
    Base manufacture(int, int);
};

class SubFactory {
public:
    Sub manufacture();
};

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct_with) {
    bool ret = Internals::can_construct_with<BaseFactory, Base, int, int>();
    BOOST_CHECK(ret);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_construct) {
    bool ret = Internals::can_construct<SubFactory, Sub>();
    BOOST_CHECK(ret);
}

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_int) {
    bool ret = Internals::can_stream<int>();
    BOOST_CHECK_MESSAGE(ret, "can_stream failed for built-in type `int`");
}

BOOST_AUTO_TEST_CASE(Test_Utils_ConstExpr_can_stream_bool) {
    bool ret = Internals::can_stream<bool>();
    BOOST_CHECK_MESSAGE(ret, "can_stream failed for built-in type `bool`");
}

#pragma clang diagnostic pop

