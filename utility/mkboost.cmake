message(STATUS "Installing Project-level Boost")

# Download archive if not existing
if ((NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz") AND (NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0"))
    message(STATUS ">Downloading archive")
    if (BODANDS_INTERNET_IS_FADING_AWAY)
        file(COPY D:/boost_1_70_0.tar.gz DESTINATION ${CMAKE_SOURCE_DIR})
    else ()
        file(DOWNLOAD
                https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.gz
                ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz
                EXPECTED_HASH SHA256=882b48708d211a5f48e60b0124cf5863c1534cd544ecd0664bb534a4b5d506e9
                )
    endif ()
    message(STATUS ">Downloading archive - Done")
else ()
    message(STATUS ">Downloading archive - Cached")
endif ()

# Extact archive if not already done
if ((EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz") AND (NOT EXISTS "${CMAKE_SOURCE_DIR}/boost_1_70_0"))
    message(STATUS ">Extracting archive")
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
    message(STATUS ">Extracting archive - Done")
else ()
    message(STATUS ">Extracting archive - Cached")
endif ()

# Delete archive if existing
if (EXISTS ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz)
    message(STATUS ">Deleting archive")
    file(REMOVE ${CMAKE_SOURCE_DIR}/boost_1_70_0.tar.gz)
    message(STATUS ">Deleting archive - Done")
endif ()

# Add include directory
message(STATUS ">Registering boost path")
file(GLOB AUTO_BOOST_DIR RELATIVE ${CMAKE_SOURCE_DIR} boost_*)
include_directories("${AUTO_BOOST_DIR}")
message(STATUS ">Registering boost path - Done")

message(STATUS "Installing Project-level Boost - Done")
