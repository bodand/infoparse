/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019. 05. 01..
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <tuple>
#include <random>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include "../src/Option_.hpp"

using namespace info::parse;
using namespace info::parse::detail;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(Test_Option)
  typedef std::tuple<std::string, std::string, std::string> strings;

  strings mkParseString(bool useShort, bool useNoise,
                        const std::string& valSep,
                        const std::string& value) {
      std::string noise(useNoise ? "--whatever --overthrow humanity "
                                 : "");
      std::string begin("text " + noise + "text ");
      std::string end(" text " + noise + "text");
      std::string parse((useShort ? "-c"
                                  : "--test")
                        + valSep + value);
      return {
              begin + parse + end,
              begin + end.substr(1),
              begin + value + end
      };
  }

  auto valSeps = bdata::make({
                                     std::make_tuple(std::string(""), true),
                                     std::make_tuple(std::string(" "), true),
                                     std::make_tuple(std::string("="), true),
                                     std::make_tuple(std::string("= "), false),
                                     std::make_tuple(std::string(":"), true),
                                     std::make_tuple(std::string(": "), true)
                             });

  BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithDeductedShortName) {
      int i = 0;
      auto opt = new Option_("test", &i);
      BOOST_CHECK_NE(opt, nullptr);
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithExplicitShortName) {
      int i = 0;
      auto opt = new Option_("test|c", &i);
      BOOST_CHECK_NE(opt, nullptr);
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameSole,
                       valSeps, sample, match) {
      int i = 0;
      auto opt = new Option_("test|c", &i);
      auto[matchee, exp, fail] = mkParseString(false, false, sample, "4");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(i, 4);
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(i, 0);
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameSole,
                       valSeps, sample, match) {
      int i = 0;
      auto opt = new Option_("test|c", &i);
      auto[matchee, exp, fail] = mkParseString(true, false, sample, "4");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(i, 4);
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(i, 0);
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameSole,
                       valSeps, sample, match) {
      std::string s;
      auto opt = new Option_("test|c", &s);
      auto[matchee, exp, fail] = mkParseString(false, false, sample, "string");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(s, "string");
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(s, "");
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameSole,
                       valSeps, sample, match) {
      std::string s;
      auto opt = new Option_("test|c", &s);
      auto[matchee, exp, fail] = mkParseString(true, false, sample, "string");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(s, "string");
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(s, "");
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameCherryPick,
                       valSeps, sample, match) {
      int i = 0;
      auto opt = new Option_("test|c", &i);
      auto[matchee, exp, fail] = mkParseString(false, true, sample, "42");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(i, 42);
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(i, 0);
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameCherryPick,
                       valSeps, sample, match) {
      int i = 0;
      auto opt = new Option_("test|c", &i);
      auto[matchee, exp, fail] = mkParseString(true, true, sample, "42");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(i, 42);
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(i, 0);
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameCherryPick,
                       valSeps, sample, match) {
      std::string s;
      auto opt = new Option_("test|c", &s);
      auto[matchee, exp, fail] = mkParseString(false, true, sample, "string");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(s, "string");
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(s, "");
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_DATA_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameCherryPick,
                       valSeps, sample, match) {
      std::string s;
      auto opt = new Option_("test|c", &s);
      auto[matchee, exp, fail] = mkParseString(true, true, sample, "string");
      auto res = opt->match(matchee);
      if (match) {
          BOOST_CHECK_EQUAL(s, "string");
          BOOST_CHECK_EQUAL(res, exp);
      } else {
          BOOST_CHECK_EQUAL(s, "");
          BOOST_CHECK_EQUAL(res, fail);
      }
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionEquality) {
      int i = 0;
      auto* optc = new Option_("test", &i);
      auto* optk = new Option_("test", &i);
      BOOST_CHECK_EQUAL(*optc, *optk);
      delete optc;
      delete optk;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionEqualityBasedOnLONGNamesFalse) {
      int i = 0;
      auto* optc = new Option_("testc", &i);
      auto* optk = new Option_("testk", &i);
      BOOST_CHECK_NE(*optc, *optk);
      delete optc;
      delete optk;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ShortFlagsParsedCorrectly) {
      bool b;
      auto* opt = new Option_("test|t", &b);
      auto res = opt->match("text text -t text text");
      BOOST_CHECK_MESSAGE(b, "Boolean short flag parsed correctly");
      BOOST_CHECK_EQUAL(res, "text text text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_LongFlagsParsedCorrectly) {
      bool b;
      auto* opt = new Option_("test", &b);
      auto res = opt->match("text text --test text text");
      BOOST_CHECK_MESSAGE(b, "Boolean long flag parsed correctly");
      BOOST_CHECK_EQUAL(res, "text text text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_LongFlagsParsedCorrectlyAsLastInString) {
      bool b;
      auto* opt = new Option_("test", &b);
      auto res = opt->match("text text text text --test");
      BOOST_CHECK_MESSAGE(b, "Boolean long flag parsed correctly as last");
      BOOST_CHECK_EQUAL(res, "text text text text ");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_InvalidLongFlagNotParsed) {
      bool b;
      auto* opt = new Option_("test", &b);
      auto res = opt->match("text text --testtext text");
      BOOST_CHECK_MESSAGE(!b, "Invalid Boolean long flag not parsed");
      BOOST_CHECK_EQUAL(res, "text text --testtext text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_InvalidShortFlagNotParsed) {
      bool b;
      auto* opt = new Option_("test", &b);
      auto res = opt->match("text text -ttext text");
      BOOST_CHECK_MESSAGE(!b, "Invalid Boolean short flag not parsed");
      BOOST_CHECK_EQUAL(res, "text text -ttext text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_NegatedLongFlagInvocation) {
      bool no;
      auto* opt = new Option_("flag", &no);
      auto res = opt->match("text text --no-flag text text");
      BOOST_CHECK_MESSAGE(!no, "Negated long flag invocation --no-flag parsed correctly");
      BOOST_CHECK_EQUAL(res, "text text text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationLongYesNo) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag=yes text --nflag=no text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "Yes parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "No parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationLongTrueFalse) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag=true text --nflag=false text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "True parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "False parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationLong10) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag=1 text --nflag=0 text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "1 parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "0 parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationLongArbirtaryTruthynessCheck) {
      bool num, text, noNum, noTxt;
      auto* numOpt = new Option_("nflag", &num);
      auto* txtOpt = new Option_("tflag", &text);
      auto* nNmOpt = new Option_("nnflag", &noNum);
      auto* nTxOpt = new Option_("ntflag", &noTxt);
      std::string parsee("text --nflag=14Miles text --tflag=text text --nnflag=0deaths --ntflag= text");
      auto numRes = numOpt->match(parsee);
      auto txtRes = txtOpt->match(numRes);
      auto nNmRes = nNmOpt->match(txtRes);
      auto res = nTxOpt->match(nNmRes);
      BOOST_CHECK_MESSAGE(num, "Truthy numeric value parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(text, "Truthy string value correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!noNum, "Falsy numeric value parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!noTxt, "Falsy string value parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text text");
      delete txtOpt;
      delete numOpt;
      delete nNmOpt;
      delete nTxOpt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationShortYesNo) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y=yes text -n=no text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "Yes parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "No parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationShortTrueFalse) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y=true text -n=false text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "True parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "False parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationShort10) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y=1 text -n=0 text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "1 parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "0 parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ExtendedFlagInvocationShortArbirtaryTruthynessCheck) {
      bool num, text, noNum, noTxt;
      auto* numOpt = new Option_("nflag|n", &num);
      auto* txtOpt = new Option_("tflag|t", &text);
      auto* nNmOpt = new Option_("nnflag|x", &noNum);
      auto* nTxOpt = new Option_("ntflag|y", &noTxt);
      std::string parsee("text -n=14Miles text -t=text text -x=0deaths -y= text");
      auto numRes = numOpt->match(parsee);
      auto txtRes = txtOpt->match(numRes);
      auto nNmRes = nNmOpt->match(txtRes);
      auto res = nTxOpt->match(nNmRes);
      BOOST_CHECK_MESSAGE(num, "Truthy numeric value parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(text, "Truthy string value correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!noNum, "Falsy numeric value parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!noTxt, "Falsy string value parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text text");
      delete txtOpt;
      delete numOpt;
      delete nNmOpt;
      delete nTxOpt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationLongYesNo) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag: yes text --nflag: no text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "Yes parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "No parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationLongTrueFalse) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag: true text --nflag: false text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "True parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "False parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationLong10) {
      bool yes, no;
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
      std::string parsee("text --yflag: 1 text --nflag: 0 text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "1 parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!no, "0 parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationLongArbirtaryTruthynessCheck) {
      bool num, text, noNum, noTxt;
      auto* numOpt = new Option_("nflag", &num);
      auto* txtOpt = new Option_("tflag", &text);
      auto* nNmOpt = new Option_("nnflag", &noNum);
      auto* nTxOpt = new Option_("ntflag", &noTxt);
      std::string parsee("text --nflag:    14Miles text --tflag: text text"
                         " --nnflag: 0deaths --ntflag:");
      auto numRes = numOpt->match(parsee);
      auto txtRes = txtOpt->match(numRes);
      auto nNmRes = nNmOpt->match(txtRes);
      auto res = nTxOpt->match(nNmRes);
      BOOST_CHECK_MESSAGE(num, "Truthy numeric value parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(text, "Truthy string value correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!noNum, "Falsy numeric value parsed correctly in extended flag invocation");
      BOOST_CHECK_MESSAGE(!noTxt, "Falsy string value parsed correctly in extended flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text ");
      delete txtOpt;
      delete numOpt;
      delete nNmOpt;
      delete nTxOpt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationShortYesNo) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y:yes text -n: no text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "Yes parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "No parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationShortTrueFalse) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y:true text -n: false text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "True parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "False parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationShort10) {
      bool yes, no;
      auto* yopt = new Option_("yflag|y", &yes);
      auto* nopt = new Option_("nflag|n", &no);
      std::string parsee("text -y:1 text -n:0 text");
      auto yres = yopt->match(parsee);
      auto res = nopt->match(yres);
      BOOST_CHECK_MESSAGE(yes, "1 parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!no, "0 parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text");
      delete nopt;
      delete yopt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_SeekingExtendedFlagInvocationShortArbirtaryTruthynessCheck) {
      bool num, text, noNum, noTxt;
      auto* numOpt = new Option_("nflag|n", &num);
      auto* txtOpt = new Option_("tflag|t", &text);
      auto* nNmOpt = new Option_("nnflag|x", &noNum);
      auto* nTxOpt = new Option_("ntflag|y", &noTxt);
      std::string parsee("text -n:    14Miles text -t: text text -x: 0deaths -y:");
      auto numRes = numOpt->match(parsee);
      auto txtRes = txtOpt->match(numRes);
      auto nNmRes = nNmOpt->match(txtRes);
      auto res = nTxOpt->match(nNmRes);
      BOOST_CHECK_MESSAGE(num, "Truthy numeric value parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(text, "Truthy string value correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!noNum, "Falsy numeric value parsed correctly in extended short flag invocation");
      BOOST_CHECK_MESSAGE(!noTxt, "Falsy string value parsed correctly in extended short flag invocation");
      BOOST_CHECK_EQUAL(res, "text text text ");
      delete txtOpt;
      delete numOpt;
      delete nNmOpt;
      delete nTxOpt;
  }

BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
