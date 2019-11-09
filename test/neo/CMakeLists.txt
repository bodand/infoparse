project(InfoParseNeoTest
        VERSION 1
        LANGUAGES CXX)

enable_testing()

set(INFO_PARSE_NEO_TEST_CASES
        Test_Testing.hpp
        )

configure_file(
    "${INFO_PARSE_NEO_TESTS_DIR}/main.cpp.in"
    "${INFO_PARSE_NEO_TESTS_DIR}/main.cpp"
)

add_executable(
    ipn_test
    main.cpp
)

target_link_libraries(ipn_test
    boost
    infoparseneo
)

add_test(
    TestSuiteIPN
    "ipn_test -v"
)
