/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-02-06.
//
#pragma once

#include <typeinfo>
#include <typeindex>
#include <string>
#include <map>
#include <memory>
#include <iterator>
#include <regex>

#include "utils.hpp"
#include "OptionHandler_.hpp"
#include "OptionString.hpp"

/**
 * Main namespace for the library.
 *
 * Contains all the API the library provides
 * in the form of classes and/or functions.
 */
namespace InfoParse {
  using Internals::OptionHandler_;

  class OptionsParser;
  /**
   * Contains implementation
   * details of the library.
   *
   * Nothing it contains should be used by
   * any user of the library, albeit possible.
   */
  namespace Internals {
    /**
     * Adds options to an OptionsParser
     * objects using the operator()(OptionsString, T*).
     *
     * Allows the OptionsParser::addOptions()
     * function to exist, as that function returns a
     * object of this class, which then can be used for
     * adding multiple options to the "mother" OptionParser
     * object.
     *
     * @see OptionsParser::addOptions()
     * @see operator()()
     */
    class OptionAdder {
        /// Interface
    public:
        /**
         * Adds option to the mother object by callit her
         * addOption(OptionString, T*) method.
         *
         * @tparam T The type of variable to be spit back
         * @param[in] name The name of the variable in the form
         *                  of an OptionString object.
         * @param[out] val A pointer to an object of type T which
         *                  supports operator>>
         *
         * @return A reference to this object to allow chain-calling
         *          operator()()
         *
         * @see OptionsParser::addOption()
         */
        template<class T>
        const OptionAdder& operator()(OptionString name, T* val) const;

        /// Lifecycle
    public:
        /**
         * Constructs the OptionAdder object
         * with a pointer to its mother.
         *
         * @param[in] parser The mother object
         *                    of the OptionAdder object.
         *                    Options are added to this
         *                    object by operator()()
         *
         * @see OptionAdder::operator()()
         */
        OptionAdder(OptionsParser* parser);

        /// Fields
    private:
        /// The "mother" object to add the options to
        OptionsParser* mother;
    };
  }
  using Internals::OptionAdder;

  /**
   * Stores options for parsing, and parses.
   */
  class OptionsParser {
      /// Interface
  public:
      /**
       * Adds multiple options to the parser.
       *
       * Returns an instance of an internal OptionAdder object
       * which offers an operator() which returns a reference
       * to object allowing chain calling.
       *
       * @code
       * parser.addOptions()
       *    ("alpha|a", &a)
       *    ("beta|b", &b)
       *    ("silent|quiet|s|q", &stfu)
       * ;
       * @endcode
       *
       * @return An object to call operator() on.
       *
       * @see Internals::OptionAdder
       */
      OptionAdder addOptions();

      /**
       * Adds an option with T type parameter to be
       * stored and later invoked to do its parsing
       *
       * @tparam T Type for the exported value
       * @param[in] name The LONG name of the option. SHORT name is
       *             deduced from the first character of the LONG name.
       * @param[out] exporter A pointer to a memory block of type T, into
       *                 which the parsed value will be put
       *
       * @note `nullptr` for exporter is not checked, yet
       * @note T must support operator>> from istream, this
       *       is made sure by SFINAE so it will die compile time
       * @note Option names are not checked, so one of the options
       *        will shadow the other, depending their position in the std::map
       */
      template<class T>
      std::enable_if_t<Internals::can_stream<T>()
                       && std::is_default_constructible_v<T>,
              OptionsParser*>
      addOption(Internals::OptionString name, T* exporter);

      /**
       * Parses the given arguments using parameters in
       * the style of `int main` parameters.
       *
       * @param[in] argc The length of argv
       * @param[in] argv An array of char arrays which store the
       *             parameters split up by the local shell
       * @returns The remnants of the parsed concatenated string,
       *         matched options removed.
       *
       * @note In the future the return value might change to return
       *       a pair of int and char**
       * @note argv is not checked for `nullptr`
       * @note for any i < argc; argv[i] is not checked for `nullptr`
       */
      std::string parse(int argc, char** argv);

      /**
       * Parses the given string as if it was directly input from
       * the local shell
       *
       * @param[in] args The string to parse
       * @return The remnants of the parsed string, matched options removed.
       */
      std::string parse(const std::string& args);

      /// Fields
  private:
      std::map<std::type_index, std::pair<void*,
              std::function<std::string(void*, const std::string&)>>> optionHandlers;

      /// Methods & stuff
  private:
      _pure std::string explodeBundledFlags(const std::string& args);
      _pure std::string equalizeWhitespace(const std::string& args);
  };

  template<class T>
  const Internals::OptionAdder&
  Internals::OptionAdder::operator()(Internals::OptionString name,
                                     T* val) const {
      mother->addOption(std::move(name), val);
      return *this;
  }

  template<class T>
  inline std::enable_if_t<Internals::can_stream<T>()
                          && std::is_default_constructible_v<T>,
          OptionsParser*>
  OptionsParser::addOption(Internals::OptionString name, T* exporter) {
      if (optionHandlers.find(typeid(T)) == optionHandlers.end()) {
          optionHandlers[typeid(T)].first = (void*) new OptionHandler_<T>();
          optionHandlers[typeid(T)].second = [](void* optionVoid, const std::string& args) {
            return ((OptionHandler_<T>*) optionVoid)->handle(args);
          };
      }
      ((OptionHandler_<T>*) optionHandlers[typeid(T)].first)->addOption(name, exporter);
      return this;
  }

  inline std::string OptionsParser::parse(const std::string& args) {
      std::string parsable(equalizeWhitespace(explodeBundledFlags(args)));
      for (const auto& handler : optionHandlers) {
          parsable = handler.second.second(handler.second.first, parsable);
      }
      return parsable;
  }

  inline std::string OptionsParser::parse(int argc, char** argv) {
      return parse(InfoParse::makeMonolithArgs(argc, argv));
  }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
#pragma ide diagnostic ignored "OCDFAInspection"

  inline std::string OptionsParser::explodeBundledFlags(const std::string& args) {
      bool _ = true;
      std::string parsable(args);
      std::size_t bundleStart = 0;
      std::string bundleSequence(" -");

      //&!off
      for (;_;) {
      //&!on
          bundleStart = parsable.find(bundleSequence, bundleStart);
          if (bundleStart == -1) break;
          if (parsable[bundleStart + 2] == '-' && ++bundleStart) continue;

          std::size_t bundleEnd = parsable.find(' ', bundleStart + 1);
          std::size_t bundleSize = bundleEnd - bundleStart - 1;
          if (bundleSize <= 1 && ++bundleStart) continue;

          std::string bundle = parsable.substr(bundleStart, bundleSize + 1);
          parsable.erase(bundleStart, bundleSize + 1);
          std::stringstream explodedBundleStream;
          for (const auto& ch : bundle) {
              unless (ch == ' ' || ch == '-') {
                  explodedBundleStream << " -" << ch << ' ';
              }
          }
          std::string explodedBundle = explodedBundleStream.str();

          parsable.insert(bundleStart, explodedBundle);
          bundleStart++;
      }

      return parsable;
  }

#pragma clang diagnostic pop

  inline std::string OptionsParser::equalizeWhitespace(const std::string& args) {
      return std::regex_replace(args, std::regex("\\s+"), " ");
  }

  inline OptionAdder OptionsParser::addOptions() {
      return OptionAdder(this);
  }

}
