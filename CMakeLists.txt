#
# Copyright (c) 2019, András Bodor
# Licensed under the BSD 3-Clause license
# For more information see the supplied
# LICENSE file
#
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
project(InfoParse)

set(InfoParse_VERSION_MAJOR 1)
set(InfoParse_VERSION_MINOR 3)
set(InfoParse_VERSION_PATCH 5)
set(InfoParse_VERSION "${InfoParse_VERSION_MAJOR}.${InfoParse_VERSION_MINOR}.${InfoParse_VERSION_PATCH}")

configure_file(
        "${PROJECT_SOURCE_DIR}/src/versioning.hpp.in"
        "${PROJECT_SOURCE_DIR}/src/versioning.hpp"
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(USE_CLANG_PARSING "YES")
    set(COMPILATION_DB_PATH ${PROJECT_BINARY_DIR}/compile_command.json)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    set(USE_CLANG_PARSING "NO")
    set(COMPILATION_DB_PATH "")
endif ()

configure_file(
        ${PROJECT_SOURCE_DIR}/.doxygen.in
        ${PROJECT_SOURCE_DIR}/.doxygen
        @ONLY
)

message(STATUS "[InfoParse] Looking for a presinstalled Boost")
find_package(Boost COMPONENTS unit_test_framework)
if (Boost_FOUND)
    set(USE_AUTO_INSTALLED_BOOST 0)
    message(STATUS "[InfoParse] Found a preinstalled Boost")
else ()
    set(USE_AUTO_INSTALLED_BOOST 1)
    message(STATUS "[InfoParse] Did not find a preinstalled Boost")
    message(STATUS "[InfoParse] Installing the latest Boost Headers for project-level comsumption")
    if ((NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz") AND (NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0"))
        message(STATUS "Downloading archive")
        file(DOWNLOAD
                https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.gz
                ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz
                EXPECTED_HASH SHA256=882b48708d211a5f48e60b0124cf5863c1534cd544ecd0664bb534a4b5d506e9
                SHOW_PROGRESS
                )
    else ()
        message(STATUS "Already downloaded or installed")
    endif ()
    if ((EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz") AND (NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0"))
        message(STATUS "Extracting archive")
        if (WIN32)
            execute_process(
                    COMMAND cmd /c ${CMAKE_COMMAND} -E tar xzf boost_1_70_0.tar.gz
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
        else ()
            execute_process(
                    COMMAND ${CMAKE_COMMAND} -E tar xzf boost_1_70_0.tar.gz
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
        endif ()
    else ()
        message(STATUS "Already installed")
    endif ()
    if (EXISTS ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz)
        message(STATUS "Deleting archive")
        file(REMOVE ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz)
    endif ()
    file(GLOB AUTO_BOOST_DIR RELATIVE ${CMAKE_SOURCE_DIR} boost_*)
    include_directories("${AUTO_BOOST_DIR}")
endif ()


set(InfoParse_SOURCES
        src/versioning.cpp src/utils.cpp
        src/Option_.cpp
        src/OptionHandler_.cpp
        src/OptionString.cpp
        src/OptionsParser.cpp
        src/Lazy.cpp
        )

set(InfoParse_HEADERS
        # Config'd
        src/versioning.hpp
        # Procedural headers
        src/utils.hpp
        # Classes
        src/Option_.hpp
        src/OptionHandler_.hpp
        src/OptionsParser.hpp
        src/OptionString.hpp
        src/Lazy.hpp
        )

add_library(infoparse SHARED ${InfoParse_HEADERS} ${InfoParse_SOURCES})
set_target_properties(infoparse PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if (WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR CMake)
else ()
    set(DEF_INSTALL_CMAKE_DIR lib/CMake/InfoParse)
endif ()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach (header ${InfoParse_HEADERS})
    # Dynamic library
    get_target_property(pub infoparse PUBLIC_HEADER)
    if ("${pub}" STREQUAL "pub-NOTFOUND")
        set_target_properties(infoparse PROPERTIES PUBLIC_HEADER "${header}")
    else ()
        set_target_properties(infoparse PROPERTIES PUBLIC_HEADER "${header};${pub}")
    endif ()
endforeach ()

install(TARGETS infoparse EXPORT InfoParseConfig
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}/infoparse
        )
install(EXPORT InfoParseConfig DESTINATION ${INSTALL_CMAKE_DIR})
