#
# Copyright (c) 2019, András Bodor
# Licensed under the BSD 3-Clause license
# For more information see the supplied
# LICENSE file
#
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
project(InfoParse)

set(InfoParse_VERSION_MAJOR 1)
set(InfoParse_VERSION_MINOR 3)
set(InfoParse_VERSION_PATCH 5)
set(InfoParse_VERSION "${InfoParse_VERSION_MAJOR}.${InfoParse_VERSION_MINOR}.${InfoParse_VERSION_PATCH}")

configure_file(
        "${PROJECT_SOURCE_DIR}/src/versioning.hpp.in"
        "${PROJECT_SOURCE_DIR}/src/versioning.hpp"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fextended-identifiers")

message(STATUS "[InfoParse] Looking for a Presinstalled Boost")
find_package(Boost)
if (Boost_FOUND)
    message(STATUS "[InfoParse] Found a Preinstalled Boost, using Boost.Algorithm")
else()

endif()

configure_file(
        "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/src/config.hpp"
)

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
        src/config.hpp
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
