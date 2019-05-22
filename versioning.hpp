#pragma once

#define InfoParse_MAJOR 1
#define InfoParse_MINOR 2
#if defined(_WIN64) || defined(_WIN32)
  #define InfoParse_VERSION L"1.2"
#else
  #define InfoParse_VERSION "1.2"
#endif

#include <string>

namespace InfoParse {
#if defined(_WIN64) || defined(_WIN32)

    static inline const std::wstring getVersion() {
#else
        static inline const std::string getVersion() {
#endif
        return InfoParse_VERSION;
    }
}
