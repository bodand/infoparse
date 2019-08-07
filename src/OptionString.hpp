//
// Created by bodand on 2019-07-24.
//

#pragma once

#include <string>
#include <vector>

#include "Lazy.hpp"
#include "utils.hpp"

#ifdef INFO_USE_BOOST

  #include <boost/algorithm/searching/knuth_morris_pratt.hpp>
  #include <boost/algorithm/searching/boyer_moore_horspool.hpp>

#endif

namespace InfoParse::Internals {
#ifdef INFO_USE_BOOST
  using boost::algorithm::knuth_morris_pratt;
  using boost::algorithm::boyer_moore_horspool;

  template <class T>
  using FinderEins = knuth_morris_pratt<typename std::basic_string<T>::const_iterator>;
  template <class T>
  using FinderZwei = boyer_moore_horspool<typename std::basic_string<T>::const_iterator>;
#else
  template <class T>
  using FinderEins = ShittyFinder<typename std::basic_string<T>::const_iterator>;
  template <class T>
  using FinderZwei = ShittyFinder<typename std::basic_string<T>::const_iterator>;
#endif
  using StrIter = std::string::const_iterator;

  /**
   * An std::tuple containing
   * an std::string and a lazily constructed
   * Knuth-Morris-Pratt and Boyer-Moore searcher object
   */
  template<class T>
  using searchableOf = std::tuple<
          std::basic_string<T>,
          Lazy<FinderEins<T>, const std::basic_string<T>&>,
          Lazy<FinderZwei<T>, const std::basic_string<T>&>
  >;

  /**
   * Stores the acceptable
   * names of an option by which it can be
   * parsed.
   *
   * For example both short and long variants
   * of the same option are stored, however
   * arbitrary names can be stored for one option,
   * like all four for one option: `--quiet --silent -q -s`
   *
   * OptionString can be constructed implicitly from
   * a std::string and from C-String literals(const char*).
   *
   * Option names are split by pipes('|'), so the
   * earlier example can be described as the following
   * string from which the OptionString is constructed:
   * `"quiet|silent|q|s"`
   *
   * @note Empty string such as `"opt||o"` are ignored.
   */
  class OptionString {
      /// Interface
  public:
      /**
       * Returns the names stored by this object
       *
       * @return A std::vector of std::string-s containing
       *          all names with one prepended dash
       */
      _retpure const std::vector<std::string>& getNames() const;
      /**
       * Returns all names with both a lazily constructed
       * Knuth-Morris-Pratt and Boyer-Moore searcher objects
       * for their associated name.
       *
       * @return An std::vector of std::tuples each containing:
       *  - The std::string of one acceptable name for the option
       *  - A lazy Knuth-Morris-Pratt searcher provided by Boost.Algorithm
       *  - A lazy Boyer-Moore searcher provided by Boost.Algorithm
       *
       * @note The searcher objects' initialization takes
       *       the name parameter from the same tuple. Any other
       *       std::string is providable but that defeats the purpose.
       *
       * @see Internals::searchableOf<T>
       * @see OptionString::operator*()
       * @see OptionString::get()
       */
      _retpure std::vector<searchableOf<char>> get() const;
      /**
       * Return true if at least one of the
       * names is a short-type parameter (-<char>)
       */
      _retpure bool hasShort() const;

      /// Lifecycle
  public:
      /**
       * Constructs OptionString from an
       * std::string instance.
       *
       * Construction is as defined in the general class
       * description:
       *  - string is split by pipe characters ('|')
       *  - each substring is prepended with a dash
       *
       * @param[in] name The names string to be processed into names
       */
      OptionString(const std::string& name);
      /**
       * @copydoc OptionString(const std::string&)
       * @note name is not checked for `nullptr`
       */
      OptionString(const char* name);

      /// Operators
  public:
      /**
       * Returns n-th name registered for
       * the option.
       *
       * @param[in] i The index of the element to return
       * @return The i-th name of the string
       *
       * @throws std::out_of_range
       */
      _retpure const std::string& operator[](std::vector<std::string>::size_type i) const;
      /**
       * @copydoc get()
       */
      _retpure std::vector<searchableOf<char>> operator*() const;

      bool operator==(const OptionString& rhs) const;
      bool operator!=(const OptionString& rhs) const;

      /// Fields
  private:
      /// The names stored by the object
      std::vector<std::string> names;
      /// Lazily constructed Knuth-Morris-Pratt search objects for each name
      std::vector<Lazy<FinderEins<char>, const std::string&>> kmpSearch;
      /// Lazily constructed Boyer-Moore search objects for each name
      std::vector<Lazy<FinderZwei<char>, const std::string&>> bmSearch;

      /// Methods
  private:
      _pure static std::vector<std::string> prependDashes(std::vector<std::string>&& rawNames);
  };
}

