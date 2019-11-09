/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

#pragma once

#define INFO_PARSE_MAJOR 2
#define INFO_PARSE_MINOR 0
#define INFO_PARSE_PATCH 1
#define INFO_PARSE_BUILD_DATE "2019-11-09 17:41:03 (1573321263)"
#define INFO_PARSE_WIDE_BUILD_DATE L"2019-11-09 17:41:03 (1573321263)"
#define INFO_PARSE_VERSION "2.0.1"
#define INFO_PARSE_WIDE_VERSION L"2.0.1"

#include <string>
#include "config.hpp"

namespace info::parse {
  static inline std::string getVersion() {
      if (config::UseBuildTimeInVersion) {
          return INFO_PARSE_VERSION " built on " INFO_PARSE_BUILD_DATE;
      }
      return INFO_PARSE_VERSION;
  }

  static inline std::wstring getWideVersion() {
      if (config::UseBuildTimeInVersion) {
          return INFO_PARSE_WIDE_VERSION L" built on " INFO_PARSE_WIDE_BUILD_DATE;
      }
      return INFO_PARSE_WIDE_VERSION;
  }

#if defined(_WIN32) || defined(_WIN64)
  static inline std::wstring getAutoVersion() {
      return getWideVersion();
#else
  static inline std::string getAutoVersion() {
      return getVersion();
#endif
  }
}
