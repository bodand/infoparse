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
#include <unordered_map>

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

namespace info::parse {
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

  namespace detail {
    struct none {
        friend std::istream& operator>>(std::istream& is, const none& none) {
            return is;
        }
    };

    template<class It>
    class ShittyFinder {
        /// Lifecycle
    public:
        ShittyFinder(It parserB, It parserE);

        /// Operators
    public:
        std::pair<It, It> operator()(It parseeB, It parseeE) const;

        /// Fields
    private:
        It _parserB;
        It _parserE;
        std::unordered_map<typename std::iterator_traits<It>::value_type,
                std::vector<It>> _findMap;
    };

    /**
     * Checks if two supplied type's inheritance relationship
     * is that of a Base and Inheritor
     *
     * @tparam B (Base) The class from which the inheritor shall inherit
     * @tparam I (Inheritor) The class shall inherit from the base
     * @return Whether the inheritance relationship is in check
     */
    template<class B, class I>
    struct extends;

    /**
     * Helper for extends<B, I>::value
     *
     * @see extends
     */
    template<class B, class I>
    inline constexpr bool extends_v = extends<B, I>::value;

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
    struct can_construct_with;

    /**
     * Helper for can_construct_with<F, P, Args...>::value
     *
     * @see can_construct_with
     */
    template<class F, class P,
            class... Args>
    inline constexpr bool can_construct_with_v =
            can_construct_with<F, P, Args...>::value;

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
    struct can_construct;

    /**
     * Helper for can_construct<F, P>::value
     *
     * @see can_construct
     */
    template<class F, class P>
    inline constexpr bool can_construct_v = can_construct<F, P>::value;

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
    struct can_stream;

    /**
     * Helper for can_stream<T, Si, So, Args...>::value
     *
     * @see can_stream
     */
    template<class T, class Si = std::istream,
            class So = std::ostream, class... Args>
    inline constexpr bool can_stream_v = can_stream<T, Si, So, Args...>::value;

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
    struct can_stream_in;

    /**
     * Helper for can_stream_in<T, S, Args...>::value
     *
     * @see can_stream_in
     */
    template<class T, class S = std::istream, class... Args>
    inline constexpr bool can_stream_in_v = can_stream_in<T, S, Args...>::value;

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
    struct can_stream_out;

    /**
     * Helper for can_stream_out<T, S, Args...>::value
     *
     * @see can_stream_out
     */
    template<class T, class S = std::ostream, class... Args>
    inline constexpr bool can_stream_out_v = can_stream_out<T, S, Args...>::value;

    /**
     * Provides an indirection of types
     * identity<T>::type -> T
     */
    template<class T>
    struct identity;

    /**
     * Helper for identity<T>::type
     *
     * @see identity
     */
    template<class T>
    using identity_t = typename identity<T>::type;

    template<class B, class I>
    struct extends {
        static constexpr bool value =
                std::is_base_of_v<
                        typename std::remove_reference<B>::type,
                        typename std::remove_reference<I>::type
                >;
    };

    template<class F, class P, class... Args>
    struct can_construct_with {
        static constexpr bool value =
                std::is_same_v<
                        decltype(std::declval<F>().manufacture(std::declval<Args>()...)),
                        P
                >;
    };

    template<class F, class P>
    struct can_construct {
        static constexpr bool value = can_construct_with_v<F, P>;
    };

    template<class T, class S, class... Args>
    struct can_stream_in {
        template<class iT, class iS, class=void>
        struct inner : std::false_type {
        };

        template<class iT, class iS>
        struct inner<iT, iS, std::enable_if_t<
                std::is_same_v<
                        std::decay_t<decltype(std::declval<iS&>() >> std::declval<iT&>(std::declval<Args>()...))>,
                        iS
                >>
        > : std::true_type {
        };

        static constexpr bool value = inner<T, S>::value;
    };

    template<class T, class S, class... Args>
    struct can_stream_out {
        template<class iT, class iS, class=void>
        struct inner : std::false_type {
        };

        template<class iT, class iS>
        struct inner<iT, iS, std::enable_if_t<
                std::is_same_v<
                        std::decay_t<decltype(std::declval<iS&>() << std::declval<iT&>(std::declval<Args>()...))>,
                        iS
                >>
        > : std::true_type {
        };

        static constexpr bool value = inner<T, S>::value;
    };

    template<class T, class Si, class So, class... Args>
    struct can_stream {
        static constexpr bool value =
                can_stream_in_v<T, Si, Args...>
                && can_stream_out_v<T, So, Args...>;
    };

    template<class T>
    struct identity {
        using type = T;
    };

    // ShittyFinder impl

    template<class It>
    ShittyFinder<It>::ShittyFinder(It parserB, It parserE)
            : _parserB(parserB), _parserE(parserE) {
        for (auto it = parserB; it != parserE; ++it) {
            _findMap[*it].push_back(it);
        }
    }

    template<class It>
    std::pair<It, It> ShittyFinder<It>::operator()(It parseeB, It parseeE) const {
        std::size_t size = std::distance(parseeB, parseeE);
        // If nothing to find, nothing is found
        if (size == 0)
            return {_parserB, _parserB};

        typename decltype(_findMap)::const_iterator found;
        // If starter elem doesn't exist why bother
        if ((found = _findMap.find(*parseeB)) == _findMap.end())
            return {_parserE, _parserE};

        auto walkThroughShit = [](const auto& starter,
                                  const auto& matchee,
                                  const std::size_t& size) {
          for (std::size_t i = 1; i < size; ++i) {
              unless (*(matchee + i) == *(starter + i)) {
                  return false;
              }
          }
          return true;
        };

        // Check each starter elem and see if they match
        for (auto&& starter : found->second) {
            if (walkThroughShit(starter, parseeB, size))
                return {starter, starter + size};
        }

        // Fuck
        return {_parserE, _parserE};
    }
  }
}
