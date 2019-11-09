project(InfoParseClassicTest
        VERSION 1
        LANGUAGES CXX
        )

enable_testing()

set(INFO_PARSE_CLASSIC_TEST_HEADERS
        Test_Lazy.hpp
        Test_Option.hpp
        Test_OptionHandler.hpp
        Test_OptionsParser.hpp
        Test_OptionString.hpp
        Test_Testing.hpp
        Test_Utils.hpp
        )

foreach (case ${INFO_PARSE_CLASSIC_TEST_HEADERS})
    set(INFO_PARSE_CLASSIC_TEST_CASES "${INFO_PARSE_CLASSIC_TEST_CASES}
#include \"${case}\"")
endforeach ()

configure_file(
        "${INFO_PARSE_CLASSIC_TESTS_DIR}/main.cpp.in"
        "${INFO_PARSE_CLASSIC_TESTS_DIR}/main.cpp"
        )

add_executable(
        ipc_test
        main.cpp
        ${INFO_PARSE_CLASSIC_TEST_HEADERS}
        )

target_link_libraries(ipc_test
        boost
        infoparse
        )

add_test(
        TestSuiteIPC
        "ipn_test -v"
        )
