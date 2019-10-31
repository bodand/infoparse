/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-02-05.
//

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include "Option_.hpp"
#include "OptionString.hpp"

namespace info::parse::detail {

  /**
   * Groups multiple Option_<T>-s together
   * for some type T
   * Can execute handling of argument string on all contained
   * Option_<T>-s.
   * Used internally, not really useful outside of
   * the internals of the project
   *
   * @tparam T The type for Option_<T>-s can be accepted into the group
   *
   * @see info::parse::Option_<T>
   */
  template<class T = none,
          class R = none, class... Args>
  class OptionHandler_ {
      std::vector<Option_<T, R, Args...>> _options;

  public:
      /**
       * Adds an option to the internal collection
       * of Option_<T> type options
       *
       * @param[in] option The Option_<T> to be added
       */
      void addOption(Option_<T, R, Args...>&& option);

      /**
       * Constructs an Option_<T> with the supplied
       * parameters within the inner collection thus
       * skipping a move operation.
       *
       * @param[in] names The names of the param split by '|'
       * @param[out] exporter A pointer to a memory block of type T, into
       *                 which the value will be put
       */
      void addOption(OptionString names, T* exporter);

      /**
       * @copydoc addOption
       */
      void addOption(OptionString names, const std::function<R(Args...)>& func);

      /**
       * Triggers the execution of argument processing on all
       * stored Option_<T>-s, each following the other in insertion
       * order.
       *
       * @param[in] args The argument string to process
       * @return The remains of the processed string
       */
      std::string handle(const std::string& args) const;

      /**
       * Getter for the options
       * @return The internal collection of options
       */
      const std::vector<Option_<T>>& options() const;

      /**
       * Removes the option with "name".
       * No-op if it is not found internally
       * @param[in] name
       */
      void rmOption(const std::string& name);
  };

  template<class T, class R, class... Args>
  void OptionHandler_<T, R, Args...>::addOption(Option_<T, R, Args...>&& option) {
      _options.push_back(std::move(option));
  }

  template<class T, class R, class... Args>
  void OptionHandler_<T, R, Args...>::addOption(OptionString name, T* exporter) {
      _options.emplace_back(std::move(name), exporter);
  }

  template<class T, class R, class... Args>
  void OptionHandler_<T, R, Args...>::addOption(OptionString name,
                                                const std::function<R(Args...)>& func) {
      _options.emplace_back(std::move(name), func);
  }

  template<class T, class R, class... Args>
  std::string OptionHandler_<T, R, Args...>::handle(const std::string& args) const {
      std::string parsable(args);
      std::for_each(_options.begin(), _options.end(), [&](const Option_<T, R, Args...>& opt) {
        parsable = opt.match(parsable);
      });
      return parsable;
  }

  template<class T, class R, class... Args>
  const std::vector<Option_<T>>& OptionHandler_<T, R, Args...>::options() const {
      return _options;
  }

  template<class T, class R, class... Args>
  void OptionHandler_<T, R, Args...>::rmOption(const std::string& name) {
      auto i = _options.begin();
      while ((*i != name && i != _options.end())) { ++i; };
      unless (i == _options.end()) {
          _options.erase(i);
      }
  }
}


