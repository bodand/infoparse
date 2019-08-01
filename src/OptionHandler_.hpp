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

namespace InfoParse::Internals {

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
   * @see InfoParse::Option_<T>
   */
  template<class T>
  class OptionHandler_ {
      std::vector<Option_<T>> options;

  public:
      /**
       * Adds an option to the internal collection
       * of Option_<T> type options
       *
       * @param option The Option_<T> to be added
       */
      void addOption(Option_<T>&& option);

      /**
       * Constructs an Option_<T> with the supplied
       * parameters within the inner collection thus
       * skipping a move operation.
       *
       * @param names The names of the param split by '|'
       * @param exporter A pointer to a memory block of type T, into
       *                 which the value will be put
       */
      void addOption(OptionString names, T* exporter);

      /**
       * Triggers the execution of argument processing on all
       * stored Option_<T>-s, each following the other in insertion
       * order.
       *
       * @param args The argument string to process
       * @return The remains of the processed string
       */
      std::string handle(const std::string& args) const;

      /**
       * Getter for the options
       * @return The internal collection of options
       */
      const std::vector<Option_<T>>& getOptions() const;

      /**
       * Removes the option with "name".
       * No-op if it is not found internally
       * @param name
       */
      void rmOption(const std::string& name);
  };

  template<class T>
  void OptionHandler_<T>::addOption(Option_<T>&& option) {
      options.push_back(std::move(option));
  }

  template<class T>
  std::string OptionHandler_<T>::handle(const std::string& args) const {
      std::string parsable(args);
      std::for_each(options.begin(), options.end(), [&](const Option_<T>& opt) {
        parsable = opt.match(parsable);
      });
      return parsable;
  }

  template<class T>
  void OptionHandler_<T>::addOption(OptionString name, T* exporter) {
      options.emplace_back(std::move(name), exporter);
  }

  template<class T>
  const std::vector<Option_<T>>& OptionHandler_<T>::getOptions() const {
      return options;
  }

  template<class T>
  void OptionHandler_<T>::rmOption(const std::string& name) {
      auto i = options.begin();
      while ((*i != name && i != options.end())) { ++i; };
      unless (i == options.end()) {
          options.erase(i);
      }
  }
}


