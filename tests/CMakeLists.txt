if(NOT ARBYS_BIGNUM_BUILD_TESTS)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
)

# Prevent GoogleTest from overriding compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(ARBYS_BIGNUM_TEST_SOURCES
        big_int/test_add.cpp
        big_int/test_sub.cpp
        big_int/test_mul.cpp
        big_int/test_div.cpp
        big_int/test_cmp.cpp
        big_int/test_from_string.cpp
        big_int/test_integration.cpp
        big_int/test_from_integer.cpp
)

set(ARBYS_BIGNUM_TEST_HELPER_SOURCES
    helpers/helpers.cpp
)

add_executable(arbys-bignum-tests
    ${ARBYS_BIGNUM_TEST_SOURCES}
    ${ARBYS_BIGNUM_TEST_HELPER_SOURCES}
)

target_link_libraries(arbys-bignum-tests
    PRIVATE
        arbys::bignum
        GTest::gtest_main
)

# Add sanitizers in debug mode for better error detection
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(arbys-bignum-tests PRIVATE
            -fsanitize=address,undefined
        )
        target_link_options(arbys-bignum-tests PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endif()

include(GoogleTest)
gtest_discover_tests(arbys-bignum-tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES
        LABELS "unit"
)
