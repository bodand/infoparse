/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-01-20.
//

#pragma once

#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <iterator>

#define unless(x) if (!(x))
#define until(x) while (!(x))

#define ENABLE_IF typename std::enable_if<
#define THEN(T) ,T>::type

namespace InfoParse {
  // guaranteed s = arcItrStr(itrStr(s))
  // or whatever, you get it
  void itrStr(std::string& str);
  void arcItrStr(std::string& str);
  void itrStr(std::wstring& str);
  void arcItrStr(std::wstring& str);

  std::string makeMonolithArgs(int argc, char** argv);

  template<class CharT>
  void
  replaceAll(std::basic_string<CharT>& str, const std::basic_string<CharT>& from, const std::basic_string<CharT>& to);

  std::vector<std::string> splitByWhitespace(const std::string& str);

  template<class Traits, class CharT, class UnaryFunction>
  std::basic_string<CharT> regex_replace(const std::basic_string<CharT>& s,
                                         const std::basic_regex<CharT, Traits>& re, UnaryFunction f);

  template<class BidirIt, class Traits, class CharT, class UnaryFunction>
  std::basic_string<CharT> regex_replace(BidirIt first, BidirIt last,
                                         const std::basic_regex<CharT, Traits>& re, UnaryFunction f);

  namespace Internals {

    /**
     * Checks if two supplied type's inheritance relationship
     * is that of a Base and Inheritor
     *
     * @tparam B (Base) The class from which the inheritor shall inherit
     * @tparam I (Inheritor) The class shall inherit from the base
     * @return Whether the inheritance relationship is in check
     */
    template<class B, class I>
    constexpr bool extends() {
        return std::is_base_of_v<
                typename std::remove_reference<B>::type,
                typename std::remove_reference<I>::type
        >;
    }

    /**
     * Checks if the supplied Factory can create a Product using Args
     * as arguments
     *
     * @tparam F (Factory) The factory class to construct the product
     * @tparam P (Product) The class that is to be created by the factory
     * @tparam Args Parameters for the Factory manufacture() method call
     * @return Whether the Factory can produce the Product with Args
     */
    template<class F, class P,
            class... Args>
    constexpr bool can_construct_with() {
        return std::is_same_v<
                decltype(std::declval<F>().manufacture(std::declval<Args>()...)),
                P
        >;
    }

    /**
     * Checks if the supplied Factory can create a Product
     * without arguments
     *
     * @tparam F (Factory) The factory class to construct the product
     * @tparam P (Product) The class that is to be created by the factory
     * @return Whether the Factory can produce the Product
     *
     * @see can_construct_with<F, P, Args...>()
     */
    template<class F, class P>
    constexpr bool can_construct() {
        return can_construct_with<F, P>();
    }

    /**
     * Checks whether the supplied type T can
     * be streamed into a stream of type S, using
     * operator>>.
     *
     * @tparam T The type to check streamability of
     * @tparam S The stream type to check streamability into
     * @tparam Args Arguments for constructing an instance of type T
     *
     * @return Boolean depending on operator>>(S&, T&) exits.
     */
    template<class T, class S = std::istream, class... Args>
    constexpr bool can_stream() {
        return std::is_same_v<
                std::decay_t<decltype(std::declval<S&>() >> std::declval<T&>(std::declval<Args>()...))>,
                S
        >;
    }
  }

  inline std::string makeMonolithArgs(int argc, char** argv) {
      std::ostringstream retStream;
      retStream << " ";
      for (int i = 0; i < argc; ++i) {
          std::string s(argv[i]);
          itrStr(s);
          retStream << s << " ";
      }
      return retStream.str();
  }

  template<class CharT>
  inline void replaceAll(std::basic_string<CharT>& str,
                         const std::basic_string<CharT>& from,
                         const std::basic_string<CharT>& to) {
      if (from.empty())
          return;
      size_t start_pos = 0;
      while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
          str.replace(start_pos, from.length(), to);
          start_pos += to.length();
      }
  }

  inline std::vector<std::string> splitByWhitespace(const std::string& str) {
      std::istringstream remnants(str);
      return std::vector<std::string>(std::istream_iterator<std::string>{remnants},
                                      std::istream_iterator<std::string>{});
  }

  template<class Traits, class CharT, class UnaryFunction>
  inline std::basic_string<CharT> regex_replace(const std::basic_string<CharT>& s,
                                                const std::basic_regex<CharT, Traits>& re, UnaryFunction f) {
      return regex_replace(s.cbegin(), s.cend(), re, f);
  }

  template<class BidirIt, class Traits, class CharT, class UnaryFunction>
  inline std::basic_string<CharT> regex_replace(BidirIt first, BidirIt last,
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

  inline void itrStr(std::string& str) {
      replaceAll(str, "$", R"(\$)");
      str = regex_replace(str, std::regex(R"(\s+)"),
                          [](const std::smatch& m) {
                            std::stringstream stream;
                            stream << "$" << m.length() << "$";
                            return stream.str();
                          });
  }

  inline void arcItrStr(std::string& str) {
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

  inline void itrStr(std::wstring& str) {
      replaceAll(str, L"$", LR"(\$)");
      str = regex_replace(str, std::wregex(LR"(\s+)"),
                          [](const std::wsmatch& m) {
                            std::wstringstream stream;
                            stream << L"$" << m.length() << L"$";
                            return stream.str();
                          });
  }

  inline void arcItrStr(std::wstring& str) {
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
}
