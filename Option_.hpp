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
  template<class T>
  class Option_ {
      std::string longName;
      char shortName;
      T* exporter;

  public:
      Option_(std::string longName, char shortName, T* exporter);

      Option_(const std::string& name, T* exporter);

      Option_<T>(Option_<T>& other) = default;
      Option_<T>(Option_<T>&& other) noexcept = default;
      Option_<T>& operator=(const Option_<T>& other) = default;
      Option_<T>& operator=(Option_<T>&& other) noexcept = default;


      virtual ~Option_() = default;

      std::string match(const std::string& args) const;

      template<class U>
      friend std::ostream& operator<<(std::ostream& os, const Option_<U>& option);

      bool operator==(const Option_& rhs) const;
      bool operator==(const std::string& name) const;
      bool operator==(const char* cname) const;
      bool operator!=(const Option_& rhs) const;
      bool operator!=(const std::string& name) const;
      bool operator!=(const char* cname) const;

  private:
      void handleParameterParsing(std::size_t startMatch, std::string& args,
                                  const std::string& name, const std::string& sequence) const;

      std::string _getOptionValueAsString(std::size_t startMatch, std::string& args,
                                          const std::string& name, const std::string& sequence) const;

      std::wstring _getOptionValueAsWString(std::size_t startMatch, std::string& args,
                                            const std::string& name, const std::string& sequence) const;
  };

  template<>
  class Option_<bool> {
      std::string longName;
      char shortName;
      bool* exporter;

  public:
      Option_(std::string longName, char shortName, bool* exporter);

      Option_(const std::string& name, bool* exporter);

      Option_<bool>(Option_<bool>& other) = default;
      Option_<bool>(Option_<bool>&& other) = default;
      Option_<bool>& operator=(const Option_<bool>& other) = default;
      Option_<bool>& operator=(Option_<bool>&& other) = default;

      virtual ~Option_() = default;

      std::string match(const std::string& args) const;

      template<class U>
      friend std::ostream& operator<<(std::ostream& os, const Option_<U>& option);

      bool operator==(const Option_& rhs) const;
      bool operator==(const std::string& name) const;
      bool operator==(const char* cname) const;
      bool operator!=(const Option_& rhs) const;
      bool operator!=(const std::string& name) const;
      bool operator!=(const char* cname) const;
  };

  template<class T>
  std::string Option_<T>::match(const std::string& args) const {
      std::string parsable(args);
      std::string shortNameString(std::string("") + shortName);
      std::size_t startMatch;
      const auto longSequence = std::string(" --") + longName + " ";
      const auto shortSequence = std::string(" -") + shortName + " ";

      unless ((startMatch = parsable.find(longSequence)) == -1) {
          handleParameterParsing(startMatch, /*&*/parsable, longName, longSequence);
      } else unless ((startMatch = parsable.find(shortSequence)) == -1) {
          handleParameterParsing(startMatch, /*&*/parsable, shortNameString, shortSequence);
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
      return longName == rhs.longName;
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
}

