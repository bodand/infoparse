#
# Copyright (c) 2019, András Bodor
# Licensed under BSD 3-Clause
# For more information see the supplied
# LICENSE file
#

# Testing -> Boost.Test
message(STATUS "[InfoParse] Checking for Boost for testing")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/include/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib/")
find_package(Boost COMPONENTS unit_test_framework)
if (Boost_FOUND)
    message(STATUS "[InfoParse] Found Boost, enabling testing")
    enable_testing()
    add_definitions(-DBOOST_TEST_DYN_LINK)

    set(Test_HEADERS
            Test_Testing.hpp
            Test_OptionHandler.hpp
            Test_Option.hpp
            Test_Utils.hpp
            Test_OptionsParser.hpp
            )

    foreach (case ${Test_HEADERS})
        set(Test_TEST_CASES "${Test_TEST_CASES}
#include \"${case}\"")
    endforeach ()

    configure_file(
            "${PROJECT_SOURCE_DIR}/test/testmain.cpp.in"
            "${PROJECT_SOURCE_DIR}/test/testmain.cpp"
    )

    foreach (file ${InfoParse_HEADERS})
        set(InfoParse_FILES "../${file}")
    endforeach ()

    add_executable(ip_test ${Test_HEADERS} testmain.cpp ${InfoParse_FILES})
    target_link_libraries(ip_test infoparse)
    set_target_properties(ip_test PROPERTIES LINKER_LANGUAGE CXX)
    add_test("Run-BoostTests" ip_test "--logger=HRF,all")
else ()
    message(STATUS "[InfoParse] Could not find Boost, disabling testing")
endif ()
