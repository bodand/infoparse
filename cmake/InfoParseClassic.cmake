project(InfoParseClassic
        VERSION ${INFO_PARSE_CLASSIC_VERSION}
        LANGUAGES CXX
        )

option(INFO_PARSE_CLASSIC_BUILD_TESTS "Build InfoParse Classic Test-Suite" On)
option(INFO_PARSE_CLASSIC_BUILD_SHARED "Build InfoParse Classic as a shared object or dll" On)

if (INFO_PARSE_CLASSIC_BUILD_SHARED)
    set(INFO_PARSE_CLASSIC_LIBRARY_VERSION "SHARED")
else ()
    set(INFO_PARSE_CLASSIC_LIBRARY_VERSION "STATIC")
endif ()

set(INFO_PARSE_CLASSIC_SOURCE_DIR "${INFO_PARSE_SOURCE_DIR}/classic")
set(INFO_PARSE_CLASSIC_INCLUDE_DIR "${INFO_PARSE_INCLUDE_DIR}")
set(INFO_PARSE_CLASSIC_CONFIG_DIR "${INFO_PARSE_CLASSIC_SOURCE_DIR}/config")
set(INFO_PARSE_CLASSIC_TESTS_DIR "${INFO_PARSE_TESTS_DIR}/classic")

configure_file( # Version
        "${INFO_PARSE_CLASSIC_CONFIG_DIR}/versioning.hpp.in"
        "${INFO_PARSE_CLASSIC_INCLUDE_DIR}/versioning.hpp"
        )

## Create library
add_library(infoparse ${INFO_PARSE_CLASSIC_LIBRARY_VERSION}
        ../src/classic/utils.cpp
        ../src/classic/Lazy.cpp
        ../src/classic/Option_.cpp
        ../src/classic/OptionHandler_.cpp
        ../src/classic/OptionsParser.cpp
        ../src/classic/OptionString.cpp
        )

## Include directories
target_include_directories(infoparse
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

        PRIVATE
        src
        )

target_link_libraries(infoparse
        boost
        )

## Tests
if (INFO_PARSE_CLASSIC_BUILD_TESTS)
    add_subdirectory(test/classic)
endif ()

## Misc.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(COMPILATION_DB_PATH "${PROJECT_BINARY_DIR}/compile_command.json")
endif ()

## Installation targets
install(TARGETS infoparse
        EXPORT InfoParseConfig
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        )

install(DIRECTORY "${INFO_PARSE_CLASSIC_INCLUDE_DIR}"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        )

install(EXPORT InfoParseConfig
        DESTINATION cmake
        )

export(TARGETS infoparse
        FILE InfoParseConfig.cmake
        )
