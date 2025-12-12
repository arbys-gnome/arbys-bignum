cmake_minimum_required(VERSION 4.0)
project(BigNumbers)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(include)
add_library(BigNumbers SHARED src/bigint.cpp)

enable_testing()

add_executable(bignumber_test
        tests/helpers/helpers.cpp
        tests/bigint_from_string_test.cpp
        tests/bigint_add_test.cpp
        tests/helpers/helpers.h
        tests/bigint_sub_test.cpp
)

target_link_libraries(bignumber_test
        BigNumbers
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bignumber_test)
