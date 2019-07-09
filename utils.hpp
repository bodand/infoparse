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

  void replaceAll(std::string& str, const std::string& from, const std::string& to);
  void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);

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

    template<class T, class S = std::istream, class... Args>
    constexpr bool can_stream() {
        return std::is_same_v<
                std::decay_t<decltype(std::declval<S&>() >> std::declval<T&>(std::declval<Args>()...))>,
                S
        >;
    }
  }
}
