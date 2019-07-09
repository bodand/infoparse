/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

#pragma once

#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <locale>

#include "utils.hpp"

namespace InfoParse {
  /**
   * Stores one parsable option with a SHORT
   * and LONG name.
   *
   * LONG names are parsed like "--LONG", while
   * SHORT names are parsed like "-SHORT", and SHORT
   * names are one character long.
   *
   * @tparam T The type of the parameter to stuff the
   *           found value into.
   *
   * @specialized Option_<bool>
   */
  template<class T>
  class Option_ {
      std::string longName;
      char shortName;
      T* exporter;

  public:
      /**
       * Constructs the Option_ with explicitly
       * given SHORT name.
       *
       * @param longName The LONG name for the option.
       * @param shortName The SHORT name for the option
       * @param exporter The pointer to a constructed memory whereto
       *                 dump the found value
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(std::string longName, char shortName, T* exporter);

      /**
       * Constructs the Option_ with implicitly deducted
       * SHORT name. The SHORT name is generated as the first character
       * of the LONG name.
       *
       * @param longName The LONG name for the option. First character will
       *                 used as the SHORT name
       * @param exporter The pointer to a constructed memory whereto
       *                 dump the found value
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(const std::string& name, T* exporter);

      Option_<T>(Option_<T>& other) = default;
      Option_<T>(Option_<T>&& other) noexcept = default;
      Option_<T>& operator=(const Option_<T>& other) = default;
      Option_<T>& operator=(Option_<T>&& other) noexcept = default;

      virtual ~Option_() = default;

      /**
       * Tries to match either of the names of the Option_
       * to the supplied string.
       *
       * @param args The string to match the LONG then
       *             if not found the SHORT name to
       * @return The string with the removed options' name and value
       *         For example: " text -b 4 more text" -> " text more text "
       */
      std::string match(const std::string& args) const;

      template<class U>
      friend std::ostream& operator<<(std::ostream& os, const Option_<U>& option);

      /**
       * Checks equality depending on equal
       * LONG and SHORT names
       *
       * let A = lhs.LONG == rhs.LONG
       * let B = lhs.SHORT == rhs.SHORT
       *
       * lhs == rhs iff A ∨ B
       */
      bool operator==(const Option_& rhs) const;

      /**
       * Checks equality with checking
       * whether the given string equals to LONG name
       */
      bool operator==(const std::string& name) const;

      /**
       * Checks equality with checking
       * whether the given c-string equals to LONG name
       */
      bool operator==(const char* cname) const;

      /**
       * Checks equality with checking
       * whether the given char equals to SHORT name
       */
      bool operator==(char c) const;

      /**
       * Checks inequality by negating the
       * equality check.
       * let A = lhs.LONG == rhs.LONG
       * let B = lhs.SHORT == rhs.SHORT
       *
       * lhs != rhs iff !(A ∨ B)
       */
      bool operator!=(const Option_& rhs) const;

      /**
        * Checks inequality with checking
        * whether the given string does
        * not equal to LONG name
        */
      bool operator!=(const std::string& name) const;

      /**
        * Checks inequality with checking
        * whether the given c-string does
        * not equal to LONG name
        */
      bool operator!=(const char* cname) const;

      /**
        * Checks inequality with checking
        * whether the given char does
        * not equal to SHORT name
        */
      bool operator!=(char c) const;

  private:
      void handleParameterParsing(std::size_t startMatch, std::string& args,
                                  const std::string& name, const std::string& sequence) const;

      std::string _getOptionValueAsString(std::size_t startMatch, std::string& args,
                                          const std::string& name, const std::string& sequence) const;

      std::wstring _getOptionValueAsWString(std::size_t startMatch, std::string& args,
                                            const std::string& name, const std::string& sequence) const;
  };

  /**
   * Specialized Option_ for
   * boolean values.
   *
   * Boolean values need not parse
   * after either their option's name was present
   * they simply dump true, otherwise false.
   *
   * Stores one parsable option with a SHORT
   * and LONG name.
   *
   * LONG names are parsed like "--LONG", while
   * SHORT names are parsed like "-SHORT", and SHORT
   * names are one character long.
   *
   * @see Option_<T>
   */
  template<>
  class Option_<bool> {
      std::string longName;
      char shortName;
      bool* exporter;

  public:
      /**
       * Constructs the Option_ with explicitly
       * given SHORT name.
       *
       * @param longName The LONG name for the option.
       * @param shortName The SHORT name for the option
       * @param exporter The bool pointer to dump whether the switch was found
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(std::string longName, char shortName, bool* exporter);

      /**
       * Constructs the Option_ with implicitly deducted
       * SHORT name. The SHORT name is generated as the first character
       * of the LONG name.
       *
       * @param longName The LONG name for the option. First character will
       *                 used as the SHORT name
       * @param exporter The bool pointer to dump whether the switch was found
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(const std::string& name, bool* exporter);

      Option_<bool>(Option_<bool>& other) = default;
      Option_<bool>(Option_<bool>&& other) = default;
      Option_<bool>& operator=(const Option_<bool>& other) = default;
      Option_<bool>& operator=(Option_<bool>&& other) = default;

      virtual ~Option_() = default;

      /**
        * Tries to match either of the names of the Option_
        * to the supplied string.
        *
        * @param args The string to match the LONG then
        *             if not found the SHORT name to
        * @return The string with the removed options' name
        *         For example: " text -s more text" -> " text more text "
        */
      std::string match(const std::string& args) const;

      template<class U>
      friend std::ostream& operator<<(std::ostream& os, const Option_<U>& option);

      /**
       * Checks equality depending on equal
       * LONG and SHORT names
       *
       * let A = lhs.LONG == rhs.LONG
       *
       * let B = lhs.SHORT == rhs.SHORT
       *
       * lhs == rhs iff A ∨ B
       */
      bool operator==(const Option_& rhs) const;

      /**
       * Checks equality with checking
       * whether the given string equals to LONG name
       */
      bool operator==(const std::string& name) const;

      /**
       * Checks equality with checking
       * whether the given c-string equals to LONG name
       */
      bool operator==(const char* cname) const;

      /**
       * Checks equality with checking
       * whether the given char equals to SHORT name
       */
      bool operator==(char c) const;

      /**
       * Checks inequality by negating the
       * equality check.
       *
       * let A = lhs.LONG == rhs.LONG
       *
       * let B = lhs.SHORT == rhs.SHORT
       *
       * lhs != rhs iff !(A ∨ B)
       */
      bool operator!=(const Option_& rhs) const;

      /**
        * Checks inequality with checking
        * whether the given string does
        * not equal to LONG name
        */
      bool operator!=(const std::string& name) const;

      /**
        * Checks inequality with checking
        * whether the given c-string does
        * not equal to LONG name
        */
      bool operator!=(const char* cname) const;

      /**
        * Checks inequality with checking
        * whether the given char does
        * not equal to SHORT name
        */
      bool operator!=(char c) const;
  };

  template<class T>
  std::string Option_<T>::match(const std::string& args) const {
      std::string parsable(args);
      std::string shortNameString(std::string("") + shortName);
      std::size_t startMatch;
      const auto longSequence = std::string(" --") + longName + " ";
      const auto shortSequence = std::string(" -") + shortName + " ";

      unless ((startMatch = parsable.find(longSequence)) == -1) {
          handleParameterParsing(startMatch, parsable, longName, longSequence);
      } else unless ((startMatch = parsable.find(shortSequence)) == -1) {
          handleParameterParsing(startMatch, parsable, shortNameString, shortSequence);
      }

      return parsable;
  }

  template<class T>
  Option_<T>::Option_(std::string longName, char shortName, T* exporter):
          longName(std::move(longName)),
          shortName(shortName),
          exporter(exporter) {}

  template<class T>
  Option_<T>::Option_(const std::string& name, T* exporter):
          longName(name),
          shortName(name[0]),
          exporter(exporter) {}

  template<class T>
  void Option_<T>::handleParameterParsing(std::size_t startMatch, std::string& args,
                                          const std::string& name, const std::string& sequence) const {
      if constexpr (!std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::wstring> &&
                    !std::is_same_v<std::remove_pointer_t<decltype(exporter)>, wchar_t>) {
          std::stringstream exportStream(_getOptionValueAsString(startMatch, args,
                                                                 name, sequence));
          exportStream >> *exporter;
      } else {
          std::wstringstream exportStream(_getOptionValueAsWString(startMatch, args,
                                                                   name, sequence));
          exportStream >> *exporter;
      }
      if constexpr (std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::wstring> ||
                    std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::string>) {
          arcItrStr(*exporter);
      }
  }

  template<class T>
  std::string Option_<T>::_getOptionValueAsString(std::size_t startMatch,
                                                  std::string& args,
                                                  const std::string& name,
                                                  const std::string& sequence) const {
      std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
      std::size_t matchLength = matchEnd - startMatch;
      std::string match = args.substr(startMatch, matchLength);
      args.erase(startMatch, matchLength);

      return match.substr(match.find_last_of(' ') + 1);
  }

  template<class T>
  std::wstring Option_<T>::_getOptionValueAsWString(std::size_t startMatch,
                                                    std::string& args,
                                                    const std::string& name,
                                                    const std::string& sequence) const {
      std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
      std::size_t matchLength = matchEnd - startMatch;
      std::string match = args.substr(startMatch, matchLength);
      args.erase(startMatch, matchLength);

      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

      return converter.from_bytes(match.substr(match.find_last_of(' ') + 1));
  }

  template<class U>
  std::ostream& operator<<(std::ostream& os, const Option_<U>& option) {
      os << "Option_<" << typeid(U).name() << ">[longName: " << option.longName
         << ", shortName: " << option.shortName << "]";
      return os;
  }

  template<class T>
  bool Option_<T>::operator==(const Option_& rhs) const {
      return longName == rhs.longName
             && shortName == rhs.shortName;
  }

  template<class T>
  bool Option_<T>::operator!=(const Option_& rhs) const {
      return !(rhs == *this);
  }

  template<class T>
  bool Option_<T>::operator==(const std::string& name) const {
      return longName == name;
  }

  template<class T>
  bool Option_<T>::operator!=(const std::string& name) const {
      return !(longName == name);
  }

  template<class T>
  bool Option_<T>::operator==(const char* cname) const {
      return *this == std::string(cname);
  }

  template<class T>
  bool Option_<T>::operator!=(const char* cname) const {
      return *this != std::string(cname);
  }

  template<class T>
  bool Option_<T>::operator==(char c) const {
      return shortName == c;
  }

  template<class T>
  bool Option_<T>::operator!=(char c) const {
      return !(*this == c);
  }
}

