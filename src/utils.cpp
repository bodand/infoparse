/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-01-23.
//

#include <iostream>
#include "utils.hpp"

#define BOOST_ALGORITHM_STRING_HEADER "../boost_1_70_0/boost/algorithm/string.hpp"
#include BOOST_ALGORITHM_STRING_HEADER

namespace InfoParse {
  std::string makeMonolithArgs(int argc, char** argv) {
      std::ostringstream retStream;
      retStream << " ";
      for (int i = 0; i < argc; ++i) {
          std::string s(argv[i]);
          itrStr(s);
          retStream << s << " ";
      }
      return retStream.str();
  }

  void replaceAll(std::string& str, const std::string& from, const std::string& to) {
      if (from.empty())
          return;
      size_t start_pos = 0;
      while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
          str.replace(start_pos, from.length(), to);
          start_pos += to.length();
      }
  }

  void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) {
      if (from.empty())
          return;
      size_t start_pos = 0;
      while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
          str.replace(start_pos, from.length(), to);
          start_pos += to.length();
      }
  }

  template<class Traits, class CharT, class UnaryFunction>
  std::basic_string<CharT> regex_replace(const std::basic_string<CharT>& s,
                                         const std::basic_regex<CharT, Traits>& re, UnaryFunction f) {
      return regex_replace(s.cbegin(), s.cend(), re, f);
  }

  template<class BidirIt, class Traits, class CharT, class UnaryFunction>
  std::basic_string<CharT> regex_replace(BidirIt first, BidirIt last,
                                         const std::basic_regex<CharT, Traits>& re, UnaryFunction f) {
      std::basic_string<CharT> s;

      typename std::match_results<BidirIt>::difference_type
              positionOfLastMatch = 0;
      auto endOfLastMatch = first;

      auto callback = [&](const std::match_results<BidirIt>& match) {
        auto positionOfThisMatch = match.position(0);
        auto diff = positionOfThisMatch - positionOfLastMatch;

        auto startOfThisMatch = endOfLastMatch;
        std::advance(startOfThisMatch, diff);

        s.append(endOfLastMatch, startOfThisMatch);
        s.append(f(match));

        auto lengthOfMatch = match.length(0);

        positionOfLastMatch = positionOfThisMatch + lengthOfMatch;

        endOfLastMatch = startOfThisMatch;
        std::advance(endOfLastMatch, lengthOfMatch);
      };

      std::regex_iterator<BidirIt> begin(first, last, re), end;
      std::for_each(begin, end, callback);

      s.append(endOfLastMatch, last);

      return s;
  }

  void itrStr(std::string& str) {
      replaceAll(str, "$", R"(\$)");
      str = regex_replace(str, std::regex(R"(\s+)"),
                          [](const std::smatch& m) {
                            std::stringstream stream;
                            stream << "$" << m.length() << "$";
                            return stream.str();
                          });
  }

  void arcItrStr(std::string& str) {
      str = regex_replace(str, std::regex(R"(([^\\])\$(\d+)\$)"),
                          [](const std::smatch& m) {
                            auto i = std::stoi(m.str(2));
                            std::stringstream stream;
                            stream << m.str(1);
                            for (int j = 0; j < i; ++j) {
                                stream << ' ';
                            }
                            return stream.str();
                          });
      replaceAll(str, R"(\$)", "$");
  }

  void itrStr(std::wstring& str) {
      replaceAll(str, L"$", LR"(\$)");
      str = regex_replace(str, std::wregex(LR"(\s+)"),
                          [](const std::wsmatch& m) {
                            std::wstringstream stream;
                            stream << L"$" << m.length() << L"$";
                            return stream.str();
                          });
  }

  void arcItrStr(std::wstring& str) {
      str = regex_replace(str, std::wregex(LR"(([^\\])\$(\d+)\$)"),
                          [](const std::wsmatch& m) {
                            auto i = std::stoi(m.str(2));
                            std::wstringstream stream;
                            stream << m.str(1);
                            for (int j = 0; j < i; ++j) {
                                stream << L' ';
                            }
                            return stream.str();
                          });
      replaceAll(str, LR"(\$)", L"$");
  }

  std::vector<std::string> split(const std::string& toSplit, char c) {
      std::vector<std::string> retVal;
#ifdef __clang__
      boost::split(retVal, toSplit, [&](char λc) { return λc == c; });
#else
      boost::split(retVal, toSplit, [&](char c_) { return c_ == c; });
#endif
      return retVal;
  }
}
