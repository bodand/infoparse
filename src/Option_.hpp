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

#include "config.hpp"
#include "utils.hpp"
#include "OptionString.hpp"

namespace info::parse::detail {
  /**
   * Thrown if supplied function callback
   *  1) takes too many parameters
   *  2)
   */
  class bad_function_callback : public virtual std::logic_error {
  public:
      bad_function_callback(int a);
  };

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
  template<class T = none,
          class R = none, class... Args>
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
       * Constructs the Option_ with the provided
       * names and pointer to value.
       *
       * @param[in] names The names of the param split by '|'
       * @param[out] exporter The pointer to a constructed memory whereto
       *                       dump the found value
       *
       * @note `exporter` is not checked for `nullptr`
       */
      Option_(OptionString names, T* exporter);

      /**
       * Constructs the Option_ with the provided
       * names and function callback.
       *
       * Let the input value be `f` of type `F*`.
       * `F` shall be either 1) `R(P)`; or 2) `R(P, Ps)`.<br />
       * In case of `R(P)`:
       *  - `P` shall be either 1) `void`; or 2) std or c-style string; or
       *    3) any type for which `can_stream<P>` is `true`.
       *    -# If `P` is `void`, `f` is called like `(*f)()`.
       *    -# If `P` is a string (std, or c-string) `f` will be called directly
       *       with the parsed value. The empty string is a viable value, for example
       *       in case of parsing `"--option="`.
       *    -# Otherwise according to the parsing rules used by the declared
       *       `operator>>(std::istream&, P&)`, let `val` be the value of
       *       type `P` containing the value extraced from said operator;
       *       then `f` will be called as `(*f)(val)`.
       *  - `R` shall be either 1) a `void` type; or 2) pointer type of `pR*`; or
       *    3) any type which can be cast to `int`; or 4) any type which can be converted
       *    to `bool`.
       *    -# If the `R` is a possibly cv-qualified `void`, the function is called;
       *       no diagnostic is performed and the function is believed to have
       *       succeeded.
       *    -# If `R` is a pointer type of `pR*`, then the returned value is
       *       checked for `nullptr`. If it is the function is considered to have
       *       failed.
       *    -# If `R` is convertible to `int`, the returned value will be
       *       checked for the return value as in a shell.
       *       Let `val` be the value to be passed to `f` and of type `P` as
       *       described above.
       *       If the expression `((int) (*f)(val)) == 0` is `false`, the function
       *       is considered to have failed.
       *    -# If `R` is convertible to `bool`;
       *       Let `val` be the value to be passed to `f` and of type `P` as
       *       described above.
       *       If the expression ((bool) (*f)(val)) is `false`, the function is
       *       considered to have failed.
       *    -# Otherwise no diagnostic is performed and the function is believed
       *       to have succeeded.
       *
       * In case of `R(P, Ps)`:<br />
       * `P` & `R` behave the same as it is described above for `R(P)`.
       * `Ps` is std or c-style string and the direct parsed value
       * is supplied there in the applicable type. If `Ps` is of type `pPs*`,
       * then the input value will be `((pPs*) val.c_str())`. Otherwise the library
       * tries passing in a default constructed object with the value of `Ps{}`.
       *
       * In case the function is deemed to have failed, it is run once
       * more, without any diagnostic. If it were to fail a second time,
       * it shall stay as failed, the library won't care.
       *
       * @param[in] names The names of the param split by '|'
       * @param[out] func The function callback to call with the found value
       */
      Option_(OptionString names, const std::function<R(Args...)>& func);

      /// Operators
  public:
      template<class U>
      friend std::ostream& operator<<(std::ostream& os, const Option_<U>& option);

      /**
       * Checks equality depending on equal
       * LONG and SHORT names
       *
       * let `A` = `lhs.LONG == rhs.LONG`<br />
       * let `B` = `lhs.SHORT == rhs.SHORT`<br />
       *
       * `lhs == rhs` iff `A ∨ B`
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
       *
       * let `A` be `lhs.LONG == rhs.LONG`<br />
       * let `B` be `lhs.SHORT == rhs.SHORT`<br />
       *
       * `lhs != rhs` iff `!(A ∨ B)`
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
      /// Exporter of type `T` whereto the parsed value will be spit back
      T* exporter;
      /// Optional callback-function
      std::optional<std::function<R(Args...)>> callback;
      /// Typedef of `const_iterator` of `std::string`
      typedef std::string::iterator StrCIter;

      /// Methods
  private:
      int parseFlag(std::string& parsee, std::pair<StrCIter, StrCIter> match) const;

      int parseValue(std::string& parsee, std::pair<StrCIter, StrCIter> match) const;

      int handleFlagParse(std::string& parsee, StrCIter f, StrCIter l) const;

      int handleOptionalNegatedFlagParse(std::string& parsee,
                                         StrCIter f, StrCIter l) const;

      _retpure std::string iterateNamesOnWith(std::string parsee, bool flag) const;

      void callCallback(const std::string& value) const;
  };

  template<class T, class R, class... Args>
  inline std::string Option_<T, R, Args...>::match(const std::string& args) const {
      return iterateNamesOnWith(args, std::is_same_v<T, bool>);
  }

  template<class T, class R, class... Args>
  inline Option_<T, R, Args...>::Option_(OptionString names, T* exporter)
          : exporter(exporter),
            names(std::move(names)),
            callback(std::nullopt) {}

  template<class U>
  inline std::ostream& operator<<(std::ostream& os, const Option_<U>& option) {
      os << "Option_<" << typeid(U).name() << ">[" << option.names.getNames()[0].substr(1) << "]";
      return os;
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator==(const Option_& rhs) const {
      return names == rhs.names;
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator!=(const Option_& rhs) const {
      return !(rhs == *this);
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator==(const std::string& name) const {
      return names[0] == name;
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator!=(const std::string& name) const {
      return !(name == name);
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator==(const char* cname) const {
      return *this == std::string(cname);
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator!=(const char* cname) const {
      return *this != std::string(cname);
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator==(char c) const {
      for (auto&& name : names.getNames()) {
          if (name.size() == 2 && name[1] == c)
              return true;
      }
      return false;
  }

  template<class T, class R, class... Args>
  inline bool Option_<T, R, Args...>::operator!=(char c) const {
      return !(*this == c);
  }

  template<class T, class R, class... Args>
  int Option_<T, R, Args...>::handleFlagParse(std::string& parsee,
                                              StrCIter f,
                                              StrCIter l) const {
      auto lp = std::distance(parsee.begin(), l);
      auto fp = std::distance(parsee.begin(), f);
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
              callCallback("1");
              return 1;
          case '=': {
              // +1 for we need not the =
              auto val = parsee.substr(lp + 1, parsee.find(' ', lp) - (lp + 1));
              to_lower(val);
              callCallback(std::to_string((int) evalVal(val)));
              parsee.erase(fp - bonus, lp - (fp - bonus) + 2 + val.size()); // +2 for '=' & trailing space
              return 1;
          }
          case ':': {
              if (l + 1 == parsee.end()) {
                  callCallback("0");
                  parsee.erase(fp - bonus, lp - (fp - bonus) + 1); // +1 for ':'
                  return 1;
              }
              auto firstNonSpace = parsee.find_first_not_of(' ', lp + 1); // +1 for we need not the :
              auto whitespaces = firstNonSpace - (lp + 1);
              auto endOfValue = parsee.find(' ', firstNonSpace);
              auto val = parsee.substr(firstNonSpace, endOfValue - firstNonSpace);
              to_lower(val);
              callCallback(std::to_string((int) evalVal(val)));
              parsee.erase(fp - bonus,
                           lp - (fp - bonus) + whitespaces + 2 + val.size()); // +2 for ':' & trailing space
              return 1;
          }
          default:
              if (l == parsee.end()) {
                  parsee.erase(f - bonus, l);
                  callCallback("1");
                  return 1;
              }
              return 0;
      }
  }

  template<class T, class R, class... Args>
  int Option_<T, R, Args...>::handleOptionalNegatedFlagParse(std::string& parsee,
                                                             StrCIter f,
                                                             StrCIter l) const {
      auto lp = std::distance(parsee.begin(), l);
      auto fp = std::distance(parsee.begin(), f);
      std::string noStr = "--no";
      bool match = false;
      for (int i = 0; i < noStr.size(); ++i) {
          match = *(f - (i + 1)) == noStr[noStr.size() - (i + 1)];
      }
      if (match) { // negated flag invocation
          callCallback("0");
          parsee.erase(fp - noStr.size(),
                       lp - fp + 1 + noStr.size()); //+1 for trailing space
          return 1;
      }
      // no negation just random 'o' before flag
      // note that *o-flag is invalid
      // and short _options are not negatable
      /* so just */ return 0;
  }

  template<class T, class R, class... Args>
  int Option_<T, R, Args...>::parseFlag(std::string& parsee,
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

  template<class T, class R, class... Args>
  int Option_<T, R, Args...>::parseValue(std::string& parsee,
                                         std::pair<StrCIter, StrCIter> match) const {
      auto& f = match.first;
      auto& l = match.second;
      if (f == l) { // 404
          return 0;
      }

      auto lp = std::distance(parsee.begin(), l);
      auto fp = std::distance(parsee.begin(), f);
      int bonus = fp + 2 != lp;
      int addendum = 1; // to skip ':' or leading ' '

      switch (*l) {
          case '=': {
              // +1 for we need not the =
              auto val = parsee.substr(lp + 1, parsee.find(' ', lp) - (lp + 1));
              callCallback(val);
              parsee.erase(fp - bonus, lp - (fp - bonus) + 2 + val.size()); // +2 for '=' & trailing space
              return 1;
          }
          default:
              if (l == parsee.end()) {
                  parsee.erase(f - bonus, l);
                  callCallback("");
                  return 1;
              }
              addendum--;
              [[fallthrough]];
          case ' ': [[fallthrough]];
          case ':': {
              if (l + 1 == parsee.end()) {
                  callCallback("");
                  parsee.erase(fp - bonus, lp - (fp - bonus) + addendum);
                  return 1;
              }
              auto firstNonSpace = parsee.find_first_not_of(' ', lp + addendum);
              auto whitespaces = firstNonSpace - (lp + addendum);
              auto endOfValue = parsee.find(' ', firstNonSpace);
              auto val = parsee.substr(firstNonSpace, endOfValue - firstNonSpace);
              callCallback(val);
              parsee.erase(fp - bonus,
                           lp - (fp - bonus) + whitespaces + addendum + 1 + val.size()); // +1 for trailing space
              return 1;
          }
      }
  }

  template<class T, class R, class... Args>
  std::string Option_<T, R, Args...>::iterateNamesOnWith(std::string parsee,
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

  // Do not enter unless certified Template Templar
  /****************************************************************************/
  template<class, class...>
  struct TypeD;

  template<class T1>
  constexpr std::tuple<TypeD<T1>*, int>
  mkTypeD(T1* v) {
      return {new TypeD<T1>(v), 0};
  };

  template<class T1, class... Args>
  constexpr std::tuple<TypeD<T1, Args...>*, bool>
  mkTypeD(T1 (* f)(Args...)) {
      return {new TypeD<T1, Args...>(f), false};
  }

  template<class Fst = none, class...>
  struct fP {
      using Type = Fst;
  };

  template<class = none, class Snd = none, class...>
  struct sP {
      using Type = Snd;
  };

  template<class T, class R, class... Args>
  void Option_<T, R, Args...>::callCallback(const std::string& value) const {
      if constexpr (std::is_same_v<T, none>) {
          auto checkReturnAndRetryIfNeed = [](const std::function<R(Args...)>& f,
                                              Args... args) {
//          if (config::RetryFailedCallback) {
            if constexpr (std::is_pointer_v<R>) {
                if (config::DeleteCallbackReturn) {
                    auto ptr = f(args...);
                    if (ptr == nullptr) {
                        ptr = f(args...); // Retry failed func
                    }
                    delete ptr;
                } else {
                    if (f(args...) == nullptr) {
                        f(args...); // Retry failed func
                    }
                }
            } else if constexpr (std::is_convertible_v<R, int>) {
                unless (((int) f(args...)) == 0) {
                    f(args...); // Retry failed func
                }
            } else if constexpr (std::is_convertible_v<R, bool>) {
                unless ((bool) f(args...)) {
                    f(args...); // Retry failed func
                }
            } else {
                f(args...);
            }
//          } else f();
          };

          using Arg1 = std::remove_cv_t<std::remove_reference_t<typename fP<Args...>::Type>>;
          using Arg2 = std::remove_cv_t<std::remove_reference_t<typename sP<Args...>::Type>>;

          auto makeArg = [](const std::string& value) -> Arg1 {
            if constexpr (std::is_same_v<Arg1, std::string>) {
                // String is output directly
                return value;
            }
            std::istringstream ss(value);
            Arg1 arg1;
            ss >> arg1;
            return arg1;
          };

          // Give me switch constexpr pls
          constexpr std::size_t args = sizeof...(Args);
          if constexpr (args == 0) {
              checkReturnAndRetryIfNeed(*callback);
          } else if constexpr (args == 1) {
              checkReturnAndRetryIfNeed(*callback, makeArg(value));
          } else if constexpr (args == 2) {
              if constexpr (std::is_same_v<Arg2, std::string>) {
                  // exporter takes 2 values
                  checkReturnAndRetryIfNeed(*callback, makeArg(value), value);
              } else if constexpr (std::is_pointer_v<Arg2>) {
                  // You asked for it
                  checkReturnAndRetryIfNeed(*callback, makeArg(value), (Arg2) value.c_str());
              } else // Hope this makes sense
                  checkReturnAndRetryIfNeed(*callback, makeArg(value), Arg2{});
          } else if (!config::FailSilently) {
              throw bad_function_callback(sizeof...(Args));
          }
      } else {
          auto td = mkTypeD(exporter);
          using TDType = decltype(td);
          using Typ =
          typename std::remove_pointer_t<std::tuple_element_t<0, TDType>>;
          using SecTyp = std::tuple_element_t<1, TDType>;

          if constexpr (std::is_same_v<SecTyp, bool>) {
              using Re = typename Typ::Ret;
              using Arg1 = std::remove_cv_t<std::remove_reference_t<typename Typ::Arg0>>;
              using Arg2 = std::remove_cv_t<std::remove_reference_t<typename Typ::Arg1>>;

              auto callF = [&]() -> Re {
                auto makeArg = [](const std::string& value) -> Arg1 {
                  if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<Arg1>>,
                          std::string>) {
                      // String is output directly
                      return value;
                  }
                  std::istringstream ss(value);
                  Arg1 arg1;
                  ss >> arg1;
                  return arg1;
                };

                if constexpr (std::is_same_v<Arg1, none>) {
                    // exporter takes no parameters
                    return (*exporter)();
                } else if constexpr (std::is_same_v<Arg2, none>) {
                    // exporter takes 1 parameter
                    return (*exporter)(makeArg(value));
                } else if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg2>>,
                        std::string>) {
                    // exporter takes 2 values
                    return (*exporter)(makeArg(value), value);
                } else if constexpr (std::is_pointer_v<Arg2>) {
                    // You asked for it
                    return (*exporter)(makeArg(value), (Arg2) value.c_str());
                } else
                    // Hope this makes sense
                    return (*exporter)(makeArg(value), Arg2{});
              };

              if constexpr (std::is_pointer_v<Re>) {
                  if (callF() == nullptr) {
                      callF(); // Retry failed func
                  }
              }
              if constexpr (std::is_convertible_v<Re, int>) {
                  unless (((int) callF()) == 0) {
                      callF(); // Retry failed func
                  }
              }
              if constexpr (std::is_convertible_v<Re, bool>) {
                  unless ((bool) callF()) {
                      callF(); // Retry failed func
                  }
              }
              // Not checking success
              callF();
          } else {
              if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>,
                      std::string>) {
                  // String is output directly
                  *exporter = value;
              } else if (value != "") {
                  std::istringstream ss(value);
                  ss >> *exporter;
              } else {
                  T val{};
                  *exporter = val;
              }
          }

          delete std::get<0>(td);
      }
  }

  template<class T1, class... Args>
  struct TypeD {
      typedef T1 Ret;

      template<class Fst = none, class...>
      struct fP {
          using Type = Fst;
      };

      template<class = none, class Snd = none, class...>
      struct sP {
          using Type = Snd;
      };

      using Arg0 = typename fP<Args...>::Type;
      using Arg1 = typename sP<Args...>::Type;

      typedef T1 (* func)(Args...);

      TypeD(func& f) {}

      TypeD(T1*& val) {}
  };

  /****************************************************************************/
  template<class T, class R, class... Args>
  Option_<T, R, Args...>::Option_(OptionString names, const std::function<R(Args...)>& func)
          : names(names),
            exporter(nullptr),
            callback(func) {}
}

