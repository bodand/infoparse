project(InfoParseNeo
        VERSION ${INFO_PARSE_CLASSIC_VERSION}
        LANGUAGES CXX
        )

option(INFO_PARSE_CLASSIC_BUILD_TESTS "Build InfoParse Neo Test-Suite" On)
option(INFO_PARSE_CLASSIC_BUILD_SHARED "Build InfoParse Neo as a shared object or dll" On)

if (INFO_PARSE_NEO_BUILD_SHARED)
    set(INFO_PARSE_NEO_LIBRARY_VERSION "SHARED")
else ()
    set(INFO_PARSE_NEO_LIBRARY_VERSION "STATIC")
endif ()

set(INFO_PARSE_NEO_SOURCE_DIR "${INFO_PARSE_SOURCE_DIR}/neo")
set(INFO_PARSE_NEO_INCLUDE_DIR "${INFO_PARSE_INCLUDE_DIR}/neo")
set(INFO_PARSE_NEO_CONFIG_DIR "${INFO_PARSE_NEO_SOURCE_DIR}/config")
set(INFO_PARSE_NEO_TESTS_DIR "${INFO_PARSE_TESTS_DIR}/neo")

configure_file( # Version
        "${INFO_PARSE_NEO_CONFIG_DIR}/version.hpp.in"
        "${INFO_PARSE_NEO_INCLUDE_DIR}/version.hpp"
        )

## Create library
add_library(infoparseneo ${INFO_PARSE_NEO_LIBRARY_VERSION}
        ../src/neo/StaticOption.cpp
        ../src/neo/MakeInclude.hpp
        )

## Library headers
target_include_directories(infoparseneo
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

        PRIVATE
        src
        )

target_link_libraries(infoparseneo
        boost
        )

## Tests
if (INFO_PARSE_NEO_BUILD_TESTS)
    add_subdirectory(test/neo)
endif ()

## Misc.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(COMPILATION_DB_PATH "${PROJECT_BINARY_DIR}/compile_command.json")
endif ()

## Installation targets
install(TARGETS infoparseneo
        EXPORT InfoParseNeoConfig
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        )

install(DIRECTORY "${INFO_PARSE_NEO_INCLUDE_DIR}"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        )

install(EXPORT InfoParseNeoConfig
        DESTINATION cmake
        )

export(TARGETS infoparseneo
        FILE InfoParseNeoConfig.cmake
        )
