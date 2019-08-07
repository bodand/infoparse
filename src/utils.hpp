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

#if defined(__GNUG__) && __cplusplus >= 201703L
  #define _pure [[nodiscard, gnu::const]]
  #define _retpure [[nodiscard, gnu::pure]]
  #define _retval [[nodiscard]]
#elif defined(__GNUG__) && __cplusplus >= 201103L
  #define _pure [[gnu::const]]
  #define _retpure [[gnu::pure]]
  #define _retval [[gnu::warn_unused_result]]
#elif defined(__GNUG__)
  #define _pure __attribute__((const))
  #define _retpure __attribute__((pure))
  #define _retval __attribute__ ((warn_unused_result))
#elif __cplusplus >= 201703L
  #define _pure [[nodiscard]]
  #define _retpure [[nodiscard]]
  #define _retval [[nodiscard]]
#else
  #define _pure
  #define _retpure
  #define _retval
#endif

#include "config.hpp"

#ifdef INFO_USE_BOOST

  #include <boost/algorithm/string.hpp>

#endif

namespace InfoParse {
  // guaranteed s = arcItrStr(itrStr(s))
  // or whatever, you get it
  void itrStr(std::string& str);
  void arcItrStr(std::string& str);
  void itrStr(std::wstring& str);
  void arcItrStr(std::wstring& str);

  _pure std::string makeMonolithArgs(int argc, char** argv);

  void replaceAll(std::string& str, const std::string& from, const std::string& to);
  void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);

  template<class Traits, class CharT, class UnaryFunction>
  _pure std::basic_string<CharT> regex_replace(const std::basic_string<CharT>& s,
                                               const std::basic_regex<CharT, Traits>& re,
                                               UnaryFunction f);

  template<class BidirIt, class Traits, class CharT, class UnaryFunction>
  _pure std::basic_string<CharT> regex_replace(BidirIt first, BidirIt last,
                                               const std::basic_regex<CharT, Traits>& re,
                                               UnaryFunction f);

  _pure std::vector<std::string> split(const std::string& toSplit, char c);

  _pure bool anyOf(char c, const std::string& set);

  void to_lower(std::string& str);

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
    constexpr bool extends();

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
    constexpr bool can_construct_with();

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
    constexpr bool can_construct();

    /**
     * Checks whether the supplied type T can
     * be streamed from a stream of type Si, using
     * operator>> AND can be stream into a stream of
     * type So using operator<<.
     *
     * @tparam T The type to check streamability of
     * @tparam Si The stream type to check streamability from
     * @tparam So The stream type to check streamability into
     * @tparam Args Arguments for constructing an instance of type T
     *
     * @return Boolean depending on operator>>(Si&, T&) and
     *          operator<<(So&, T&) exit.
     */
    template<class T, class Si = std::istream,
            class So = std::ostream, class... Args>
    constexpr bool can_stream();

    /**
     * Checks whether the supplied type T can
     * be streamed from a stream of type S, using
     * operator>>.
     *
     * @tparam T The type to check streamability of
     * @tparam S The stream type to check streamability from
     * @tparam Args Arguments for constructing an instance of type T
     *
     * @return Boolean depending on operator>>(S&, T&) exits.
     */
    template<class T, class S = std::istream, class... Args>
    constexpr bool can_stream_in();

    /**
     * Checks whether the supplied type T can
     * be streamed into a stream of type S, using
     * operator<<.
     *
     * @tparam T The type to check streamability of
     * @tparam S The stream type to check streamability into
     * @tparam Args Arguments for constructing an instance of type T
     *
     * @return Boolean depending on operator<<(S&, T&) exits.
     */
    template<class T, class S = std::ostream, class... Args>
    constexpr bool can_stream_out();

    template<class B, class I>
    constexpr bool extends() {
        return std::is_base_of_v<
                typename std::remove_reference<B>::type,
                typename std::remove_reference<I>::type
        >;
    }

    template<class F, class P, class... Args>
    constexpr bool can_construct_with() {
        return std::is_same_v<
                decltype(std::declval<F>().manufacture(std::declval<Args>()...)),
                P
        >;
    }

    template<class F, class P>
    constexpr bool can_construct() {
        return can_construct_with<F, P>();
    }

    template<class T, class S, class... Args>
    constexpr bool can_stream_in() {
        return std::is_same_v<
                std::decay_t<decltype(std::declval<S&>() >> std::declval<T&>(std::declval<Args>()...))>,
                S
        >;
    }

    template<class T, class S, class... Args>
    constexpr bool can_stream_out() {
        return std::is_same_v<
                std::decay_t<decltype(std::declval<S&>() << std::declval<T&>(std::declval<Args>()...))>,
                S
        >;
    }

    template<class T, class Si, class So, class... Args>
    constexpr bool can_stream() {
        return can_stream_in<T, Si, Args...>()
               && can_stream_out<T, So, Args...>();
    }

  }
}
