cmake_minimum_required(VERSION 3.10)

project(InfoParse)

option(INFO_PARSE_BUILD_CLASSIC "Build InfoParse Classic" On)
option(INFO_PARSE_BUILD_NEO "Build InfoParse Neo" On)

include(cmake/ProjectVariable.cmake)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED) # InfoParse Classic doesn't require Boost per se, but highly it is advised,
# therefore, I'm just going to stop giving a fuck
add_library(boost INTERFACE IMPORTED)
set_target_properties(boost
        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}")

## Installation fix
include(GNUInstallDirs)

if (INFO_PARSE_BUILD_NEO)
    include(cmake/InfoParseNeo.cmake)
endif ()

if (INFO_PARSE_BUILD_CLASSIC)
    include(cmake/InfoParseClassic.cmake)
endif ()
