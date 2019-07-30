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
#include <cctype>

#include <boost/algorithm/string.hpp>

#include "utils.hpp"
#include "OptionString.hpp"

namespace InfoParse::Internals {
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
   */
  template<class T>
  class Option_ {
      ///Interface
  public:
      /**
       * Tries to match either of the names of the Option_
       * to the supplied string.
       *
       * @param args The string to match the LONG then
       *             if not found the SHORT name to
       * @return The string with the removed options' name and value
       *         For example: " text -b 4 more text" -> " text more text "
       */
      _retval std::string match(const std::string& args) const;


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
       * @param name The LONG name for the option. First character will
       *                 used as the SHORT name
       * @param exporter The pointer to a constructed memory whereto
       *                 dump the found value
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(const std::string& name, T* exporter);

      /// Operators
  public:
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
      _retval bool operator==(const Option_& rhs) const;

      /**
       * Checks equality with checking
       * whether the given string equals to LONG name
       */
      _retval bool operator==(const std::string& name) const;

      /**
       * Checks equality with checking
       * whether the given c-string equals to LONG name
       */
      _retval bool operator==(const char* cname) const;

      /**
       * Checks equality with checking
       * whether the given char equals to SHORT name
       */
      _retval bool operator==(char c) const;

      /**
       * Checks inequality by negating the
       * equality check.
       * let A = lhs.LONG == rhs.LONG
       * let B = lhs.SHORT == rhs.SHORT
       *
       * lhs != rhs iff !(A ∨ B)
       */
      _retval bool operator!=(const Option_& rhs) const;

      /**
        * Checks inequality with checking
        * whether the given string does
        * not equal to LONG name
        */
      _retval bool operator!=(const std::string& name) const;

      /**
        * Checks inequality with checking
        * whether the given c-string does
        * not equal to LONG name
        */
      _retval bool operator!=(const char* cname) const;

      /**
        * Checks inequality with checking
        * whether the given char does
        * not equal to SHORT name
        */
      _retval bool operator!=(char c) const;

      /// Fields
  private:
      OptionString names;
      std::string longName;
      char shortName;
      T* exporter;

      /// Methods
  private:
      void handleParameterParsing(std::size_t startMatch, std::string& args,
                                  const std::string& name, const std::string& sequence) const;

      _retval std::string _getOptionValueAsString(std::size_t startMatch, std::string& args,
                                                  const std::string& name,
                                                  const std::string& sequence) const;

      _retval std::wstring _getOptionValueAsWString(std::size_t startMatch, std::string& args,
                                                    const std::string& name,
                                                    const std::string& sequence) const;

      _pure static bool anyOf(char c, std::string set);

      int handleFlagParse(std::string& parsee,
                          std::string::const_iterator f,
                          std::string::const_iterator l) const;

      int handleOptionalNegatedFlagParse(std::string& parsee,
                                         std::string::const_iterator f,
                                         std::string::const_iterator l) const;

      int parse(std::string& parsee,
                std::string::const_iterator f,
                std::string::const_iterator l) const;
  };

  /*
   * o --string text
   * todo --string: text -> text
   * todo --string= text -> ""
   *  --string=text      -> text
   * todo --stringtext   -> text
   * o -s text
   * todo -s=text  -> text
   * todo -s= text -> ""
   * todo -s: text -> text
   * todo -stext   -> text
   * o --int 12
   * todo x --int=12  -> 12
   * todo x --int= 12 -> 0
   * todo x --int: 12 -> 12
   * todo x --int12   -> 12
   * o -i 12
   * todo -i=12  -> 12
   * todo -i= 12 -> 0
   * todo -i: 12 -> 12
   * todo -i12   -> 12
   * o --flag
   * o --no-flag
   * o --flag=(yes|no|true|false|<truthy value like 1 or whatever>|<falsy value like 0 or "">)
   *      note: "--flag=" will set flag to false
   *  =>
   *   --flag=yes -> yes -> true
   *   --flag= yes -> "" -> false
   *   --flag: yes -> yes -> true
   *   --flag:<EOL> -> "" -> false
   * o -f
   * o -f[=:]<see above>
   */

  template<>
  inline std::string Option_<bool>::match(const std::string& args) const {
      std::string parsee(args);
      for (auto &&[name, kmp, bm] : names.get()) {
          if (name.size() > 15 || name.size() * 5 >= parsee.size()) {
              // If relative long jumps are possible use boyer-moore
              auto[first, last] = (*bm)(parsee.begin(), parsee.end());
              if (parse(parsee, first, last))
                  return parsee;
          } else {
              // otherwise fall-back on knuth-morris-pratt
              auto[first, last] = (*kmp)(parsee.begin(), parsee.end());
              if (parse(parsee, first, last))
                  return parsee;
          }
      }
      return parsee;
  }

  template<class T>
  inline std::string Option_<T>::match(const std::string& args) const {
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
  inline Option_<T>::Option_(std::string longName, char shortName, T* exporter):
          longName(std::move(longName)),
          shortName(shortName),
          exporter(exporter),
          names([&]() {
            std::ostringstream ss;
            ss << longName << "|" << shortName;
            return ss.str();
          }()) {}

  template<class T>
  inline Option_<T>::Option_(const std::string& name, T* exporter)
          : Option_(name, name[0], exporter) {}

  template<class T>
  inline void Option_<T>::handleParameterParsing(std::size_t startMatch, std::string& args,
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
  inline std::string Option_<T>::_getOptionValueAsString(std::size_t startMatch,
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
  inline std::wstring Option_<T>::_getOptionValueAsWString(std::size_t startMatch,
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
  inline std::ostream& operator<<(std::ostream& os, const Option_<U>& option) {
      os << "Option_<" << typeid(U).name() << ">[longName: " << option.longName
         << ", shortName: " << option.shortName << "]";
      return os;
  }

  template<class T>
  inline bool Option_<T>::operator==(const Option_& rhs) const {
      return longName == rhs.longName
             && shortName == rhs.shortName;
  }

  template<class T>
  inline bool Option_<T>::operator!=(const Option_& rhs) const {
      return !(rhs == *this);
  }

  template<class T>
  inline bool Option_<T>::operator==(const std::string& name) const {
      return longName == name;
  }

  template<class T>
  inline bool Option_<T>::operator!=(const std::string& name) const {
      return !(longName == name);
  }

  template<class T>
  inline bool Option_<T>::operator==(const char* cname) const {
      return *this == std::string(cname);
  }

  template<class T>
  inline bool Option_<T>::operator!=(const char* cname) const {
      return *this != std::string(cname);
  }

  template<class T>
  inline bool Option_<T>::operator==(char c) const {
      return shortName == c;
  }

  template<class T>
  inline bool Option_<T>::operator!=(char c) const {
      return !(*this == c);
  }

  template<class T>
  inline bool Option_<T>::anyOf(char c, std::string set) {
      for (auto&& cc : set) {
          if (cc == c)
              return true;
      }
      return false;
  }

  template<class T>
  int Option_<T>::handleFlagParse(std::string& parsee,
                                  std::string::const_iterator f,
                                  std::string::const_iterator l) const {
      namespace ba = boost::algorithm;
      auto lp = std::distance(parsee.cbegin(), l);
      auto fp = std::distance(parsee.cbegin(), f);
      int bonus = fp + 2 != lp;
      auto evalVal = [](std::string val) {
        if (val == "yes" || val == "true") { // true values
            return true;
        } else if (val == "no" || val == "false") { // false values
            return false;
        } else if (anyOf(val[0], "0123456789")) {
            //starts with digit means we check numeric truthyness
            return (bool) std::atoi(val.c_str());
        } else {
            //if value is only space it is falsy, otherwise truthy
            return !std::all_of(val.begin(), val.end(), ::isspace);
        }
      };

      if (*l == ' ') { // --flagtext is not valid for --flag
          parsee.erase(f - bonus, l + 1);
          *exporter = true;
          return 1;
      }
      if (l == parsee.end()) {
          parsee.erase(f - bonus, l);
          *exporter = true;
          return 1;
      }
      // before anyone asks why the fuck did I even consider and then implement
      // the ability to supply truthyness evaluation as a value to flags;
      // it exists to make this possible:
      // ./prog --use-wget=$(which wget 2>/dev/null || echo -n)
      if (*l == '=') { // extended flag invocation
          // +1 for we need not the =
          auto val = parsee.substr(lp + 1, parsee.find(' ', lp) - (lp + 1));
          ba::to_lower(val);
          *exporter = evalVal(val);
          parsee.erase(fp - bonus, lp - (fp - bonus) + 2 + val.size()); // +2 for '=' & trailing space
          return 1;
      }
      if (*l == ':') {
          if (l + 1 == parsee.end()) {
              *exporter = false;
              parsee.erase(fp - bonus, lp - (fp - bonus) + 1); // +1 for ':'
              return 1;
          }
          auto firstNonSpace = parsee.find_first_not_of(' ', lp + 1); // +1 for we need not the :
          auto whitespaces = firstNonSpace - (lp + 1);
          auto endOfValue = parsee.find(' ', firstNonSpace);
          auto val = parsee.substr(firstNonSpace, endOfValue - firstNonSpace);
          ba::to_lower(val);
          *exporter = evalVal(val);
          parsee.erase(fp - bonus,
                       lp - (fp - bonus) + whitespaces + 2 + val.size()); // +2 for ':' & trailing space
          return 1;
      }
      return 0;
  }

  template<class T>
  int Option_<T>::handleOptionalNegatedFlagParse(std::string& parsee,
                                                 std::string::const_iterator f,
                                                 std::string::const_iterator l) const {
      auto lp = std::distance(parsee.cbegin(), l);
      auto fp = std::distance(parsee.cbegin(), f);
      std::string noStr = "--no";
      bool match = false;
      for (int i = 0; i < noStr.size(); ++i) {
          match = *(f - (i + 1)) == noStr[noStr.size() - (i + 1)];
      }
      if (match) { // negated flag invocation
          *exporter = false;
          parsee.erase(fp - noStr.size(),
                       lp - fp + 1 + noStr.size()); //+1 for trailing space
          return 1;
      }
      // no negation just random 'o' before flag
      // note that *o-flag is invalid
      // and short options are not negatable
      return 0;
  }

  template<class T>
  int Option_<T>::parse(std::string& parsee,
                        std::string::const_iterator f,
                        std::string::const_iterator l) const {
      if (f == l) { // 404
          return 0;
      }

      if (*(f - 1) == '-') { // long flag invocation
          return handleFlagParse(parsee, f, l);
      } else if (*(f - 1) == 'o') { // negated flag invocation?
          return handleOptionalNegatedFlagParse(parsee, f, l);
      } else if (f + 2 == l) { // short flag
          return handleFlagParse(parsee, f, l);
      }
      return 0;
  }

}

