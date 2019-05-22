cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
project(InfoParse)

set(InfoParse_VERSION_MAJOR 1)
set(InfoParse_VERSION_MINOR 2)
set(InfoParse_VERSION "${InfoParse_VERSION_MAJOR}.${InfoParse_VERSION_MINOR}")

configure_file(
        "${PROJECT_SOURCE_DIR}/versioning.hpp.in"
        "${PROJECT_SOURCE_DIR}/versioning.hpp"
)

set(InfoParse_SOURCES
        utils.cpp _Option.cpp OptionsParser.cpp _OptionHandler.cpp)

set(InfoParse_HEADERS
        versioning.hpp utils.hpp
        _Option.hpp _OptionHandler.hpp OptionsParser.hpp)

add_library(infoparse SHARED versioning.hpp ${InfoParse_HEADERS} ${InfoParse_SOURCES})
add_library(ips STATIC versioning.hpp ${InfoParse_HEADERS} ${InfoParse_SOURCES})
set_target_properties(infoparse PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ips PROPERTIES LINKER_LANGUAGE CXX)

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
    # Static library
    get_target_property(pub infoparse PUBLIC_HEADER)
    if ("${pub}" STREQUAL "pub-NOTFOUND")
        set_target_properties(ips PROPERTIES PUBLIC_HEADER "${header}")
    else ()
        set_target_properties(ips PROPERTIES PUBLIC_HEADER "${header};${pub}")
    endif ()
endforeach ()

install(TARGETS infoparse EXPORT InfoParseConfig
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
        )
install(TARGETS ips EXPORT InfoParseConfig
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}/static
        )
install(EXPORT InfoParseConfig DESTINATION ${INSTALL_CMAKE_DIR})
