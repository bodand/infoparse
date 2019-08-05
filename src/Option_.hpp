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
       * @param[in] args The string to match the LONG then
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
       * @param[in] names The names of the param split by '|'
       * @param[out] exporter The pointer to a constructed memory whereto
       *                 dump the found value
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(OptionString names, T* exporter);

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
      /// Names of the option by which it can be parsed
      OptionString names;
      /// Exporter of type T whereto the parsed value will be spit back
      T* exporter;
      /// Typedef of const_iterator of std::string
      typedef std::string::const_iterator StrCIter;

      /// Methods
  private:
      _pure static bool anyOf(char c, std::string set);

      int parseFlag(std::string& parsee, std::pair<StrCIter, StrCIter> match) const;

      int parseValue(std::string& parsee, std::pair<StrCIter, StrCIter> match) const;

      int handleFlagParse(std::string& parsee, StrCIter f, StrCIter l) const;

      int handleOptionalNegatedFlagParse(std::string& parsee,
                                         StrCIter f, StrCIter l) const;

      _retpure std::string iterateNamesOnWith(std::string parsee, bool flag) const;
  };

  /*
   * o --string text
   * todo --string: text -> text
   * todo --string= text -> ""
   *  --string=text      -> text
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

  template<class T>
  inline std::string Option_<T>::match(const std::string& args) const {
      return iterateNamesOnWith(args, std::is_same_v<T, bool>);
  }

  template<class T>
  inline Option_<T>::Option_(OptionString names, T* exporter)
          : exporter(exporter),
            names(std::move(names)) {}

  template<class U>
  inline std::ostream& operator<<(std::ostream& os, const Option_<U>& option) {
      os << "Option_<" << typeid(U).name() << ">[" << option.names.getNames()[0].substr(1) << "]";
      return os;
  }

  template<class T>
  inline bool Option_<T>::operator==(const Option_& rhs) const {
      return names == rhs.names;
  }

  template<class T>
  inline bool Option_<T>::operator!=(const Option_& rhs) const {
      return !(rhs == *this);
  }

  template<class T>
  inline bool Option_<T>::operator==(const std::string& name) const {
      return names[0] == name;
  }

  template<class T>
  inline bool Option_<T>::operator!=(const std::string& name) const {
      return !(name == name);
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
      for (auto&& name : names.getNames()) {
          if (name.size() == 2 && name[1] == c)
              return true;
      }
      return false;
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
                                  StrCIter f,
                                  StrCIter l) const {
      namespace ba = boost::algorithm;
      auto lp = std::distance(parsee.cbegin(), l);
      auto fp = std::distance(parsee.cbegin(), f);
      int bonus = fp + 2 != lp;
      auto evalVal = [](const std::string& val) {
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

      // before anyone asks why the fuck did I even consider and then implement
      // the ability to supply truthyness evaluation as a value to flags;
      // it exists to make this possible:
      // ./prog --use-wget=$(which wget 2>/dev/null || echo -n)
      switch (*l) {
          case ' ': // --flagtext is not valid for --flag
              parsee.erase(f - bonus, l + 1);
              *exporter = true;
              return 1;
          case '=': {
              // +1 for we need not the =
              auto val = parsee.substr(lp + 1, parsee.find(' ', lp) - (lp + 1));
              ba::to_lower(val);
              *exporter = evalVal(val);
              parsee.erase(fp - bonus, lp - (fp - bonus) + 2 + val.size()); // +2 for '=' & trailing space
              return 1;
          }
          case ':': {
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
          default:
              if (l == parsee.end()) {
                  parsee.erase(f - bonus, l);
                  *exporter = true;
                  return 1;
              }
              return 0;
      }
  }

  template<class T>
  int Option_<T>::handleOptionalNegatedFlagParse(std::string& parsee,
                                                 StrCIter f,
                                                 StrCIter l) const {
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
      /* so just */ return 0;
  }

  template<class T>
  int Option_<T>::parseFlag(std::string& parsee,
                            std::pair<StrCIter, StrCIter> match) const {
      auto& f = match.first;
      auto& l = match.second;
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

  template<class T>
  int Option_<T>::parseValue(std::string& parsee,
                             std::pair<StrCIter, StrCIter> match) const {
      auto& f = match.first;
      auto& l = match.second;
      if (f == l) { // 404
          return 0;
      }
      auto evalVal = [](const std::string& val) -> T {
        std::istringstream ss(val);
        T retVal;
        ss >> retVal;
        return retVal;
      };

      namespace ba = boost::algorithm;
      auto lp = std::distance(parsee.cbegin(), l);
      auto fp = std::distance(parsee.cbegin(), f);
      int bonus = fp + 2 != lp;
      int addendum = 1; // to skip ':' or leading ' '
      using ConstructableT = std::decay_t<std::remove_pointer_t<decltype(exporter)>>;

      switch (*l) {
          case '=': {
              // +1 for we need not the =
              auto val = parsee.substr(lp + 1, parsee.find(' ', lp) - (lp + 1));
              *exporter = evalVal(val);
              parsee.erase(fp - bonus, lp - (fp - bonus) + 2 + val.size()); // +2 for '=' & trailing space
              return 1;
          }
          default:
              if (l == parsee.end()) {
                  parsee.erase(f - bonus, l);
                  *exporter = evalVal(std::to_string(ConstructableT()));
                  return 1;
              }
              addendum--;
              [[fallthrough]];
          case ' ': [[fallthrough]];
          case ':': {
              if (l + 1 == parsee.end()) {
                  *exporter = evalVal(std::to_string(ConstructableT()));
                  parsee.erase(fp - bonus, lp - (fp - bonus) + addendum);
                  return 1;
              }
              auto firstNonSpace = parsee.find_first_not_of(' ', lp + addendum);
              auto whitespaces = firstNonSpace - (lp + addendum);
              auto endOfValue = parsee.find(' ', firstNonSpace);
              auto val = parsee.substr(firstNonSpace, endOfValue - firstNonSpace);
              *exporter = evalVal(val);
              parsee.erase(fp - bonus,
                           lp - (fp - bonus) + whitespaces + addendum + 1 + val.size()); // +1 for trailing space
              return 1;
          }
      }
  }

  template<class T>
  std::string Option_<T>::iterateNamesOnWith(std::string parsee,
                                             bool flag) const {
      typedef std::string::size_type strsize_t;
      auto useBMH = []([[maybe_unused]] strsize_t parsedSize,
                       [[maybe_unused]] strsize_t parseeSize) {
        return parsedSize > 10;
      };

      auto getParser = [&](bool flag_) {
        if (flag_) {
            return &Option_::parseFlag;
        } else {
            return &Option_::parseValue;
        }
      };

      for (auto &&[name, kmp, bmh] : *names) {
          if (useBMH(name.size(), parsee.size())) {
              // If relative long jumps are possible use boyer-moore-horspool
              (this->*getParser(flag))(parsee,
                                       bmh(name)(parsee.begin(), parsee.end()));
          } else {
              // otherwise fall-back on knuth-morris-pratt
              (this->*getParser(flag))(parsee,
                                       kmp(name)(parsee.begin(), parsee.end()));
          }
      }
      return parsee;
  }

}

