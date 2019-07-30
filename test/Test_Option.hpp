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

#include <boost/test/included/unit_test.hpp>
#include "../src/Option_.hpp"

using namespace InfoParse;
using namespace InfoParse::Internals;

BOOST_AUTO_TEST_SUITE(Test_Option)

  BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithDeductedShortName) {
      int i;
      auto opt = new Option_("test", &i);
      BOOST_CHECK_NE(opt, nullptr);
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionCanBeInstantiatedWithExplicitShortName) {
      int i;
      auto opt = new Option_("test", 'c', &i);
      BOOST_CHECK_NE(opt, nullptr);
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameSole) {
      int i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text --test 4 text");
      BOOST_CHECK_EQUAL(i, 4);
      BOOST_CHECK_EQUAL(res, "text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameSole) {
      int i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text -c 4 text");
      BOOST_CHECK_EQUAL(i, 4);
      BOOST_CHECK_EQUAL(res, "text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameSole) {
      std::string s;
      auto opt = new Option_("test", 'c', &s);
      auto res = opt->match("text --test string text");
      BOOST_CHECK_EQUAL(s, "string");
      BOOST_CHECK_EQUAL(res, "text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameSole) {
      std::string s;
      auto opt = new Option_("test", 'c', &s);
      auto res = opt->match("text -c string text");
      BOOST_CHECK_EQUAL(s, "string");
      BOOST_CHECK_EQUAL(res, "text text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithLongNameCherryPick) {
      int i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text --test 42 -w whatever -o inflate --exclude humanity text");
      BOOST_CHECK_EQUAL(i, 42);
      BOOST_CHECK_EQUAL(res, "text -w whatever -o inflate --exclude humanity text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchIntWithShortNameCherryPick) {
      int i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text --explode -c 42 -w whatever -o inflate text");
      BOOST_CHECK_EQUAL(i, 42);
      BOOST_CHECK_EQUAL(res, "text --explode -w whatever -o inflate text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithLongNameCherryPick) {
      std::string i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text --test cocaine -w whatever -o inflate --exclude humanity text");
      BOOST_CHECK_EQUAL(i, "cocaine");
      BOOST_CHECK_EQUAL(res, "text -w whatever -o inflate --exclude humanity text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionShouldMatchStringWithShortNameCherryPick) {
      std::string i;
      auto opt = new Option_("test", 'c', &i);
      auto res = opt->match("text --explode -c apple -w whatever -o inflate text");
      BOOST_CHECK_EQUAL(i, "apple");
      BOOST_CHECK_EQUAL(res, "text --explode -w whatever -o inflate text");
      delete opt;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionEquality) {
      int i;
      auto* optc = new Option_("test", &i);
      auto* optk = new Option_("test", &i);
      BOOST_CHECK_EQUAL(*optc, *optk);
      delete optc;
      delete optk;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_OptionEqualityBasedOnLONGNamesFalse) {
      int i;
      auto* optc = new Option_("testc", &i);
      auto* optk = new Option_("testk", &i);
      BOOST_CHECK_NE(*optc, *optk);
      delete optc;
      delete optk;
  }

  BOOST_AUTO_TEST_CASE(Test_Option_ShortFlagsParsedCorrectly) {
      bool b;
      auto* opt = new Option_("test", &b);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* numOpt = new Option_("nflag", &num);
      auto* txtOpt = new Option_("tflag", &text);
      auto* nNmOpt = new Option_("nnflag", 'x', &noNum);
      auto* nTxOpt = new Option_("ntflag", 'y', &noTxt);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* yopt = new Option_("yflag", &yes);
      auto* nopt = new Option_("nflag", &no);
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
      auto* numOpt = new Option_("nflag", &num);
      auto* txtOpt = new Option_("tflag", &text);
      auto* nNmOpt = new Option_("nnflag", 'x', &noNum);
      auto* nTxOpt = new Option_("ntflag", 'y', &noTxt);
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
