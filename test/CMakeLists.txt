# Testing -> Boost.Test
message(STATUS "Checking for Boost for testing")
find_package(Boost COMPONENTS unit_test_framework)
if (Boost_FOUND)
    message(STATUS "Found Boost, enabling testing")
    enable_testing()
    add_definitions(-DBOOST_TEST_DYN_LINK)

    set(Test_HEADERS
            Test_Testing.hpp
            Test_Version.hpp
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
    target_link_libraries(ip_test ips)
    set_target_properties(ip_test PROPERTIES LINKER_LANGUAGE CXX)
else ()
    message(STATUS "Could not find Boost, disabling testing")
endif ()
