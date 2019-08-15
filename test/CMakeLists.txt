#
# Copyright (c) 2019, András Bodor
# Licensed under BSD 3-Clause
# For more information see the supplied
# LICENSE file
#

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL Release)
    # Testing -> Boost.Test
    enable_testing()

    set(Test_HEADERS
            Test_Testing.hpp
            Test_OptionHandler.hpp
            Test_Option.hpp
            Test_Utils.hpp
            Test_OptionsParser.hpp
            Test_Lazy.hpp
            Test_OptionString.hpp
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

    add_test("Run-BoostTests" ip_test)
endif ()
