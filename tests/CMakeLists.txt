if(NOT BIGNUMBERS_BUILD_TESTS)
    return()
endif()

# ============================================================================
# Google Test Setup
# ============================================================================

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

# ============================================================================
# Test executable
# ============================================================================

set(BIGINT_TEST_SOURCES
        bigint/test_add.cpp
        bigint/test_sub.cpp
        bigint/test_mul.cpp
        bigint/test_div.cpp
        bigint/test_cmp.cpp
        bigint/test_from_string.cpp
)

set(TEST_HELPER_SOURCES
    helpers/helpers.cpp
        bigint/test_from_integer.cpp
)

add_executable(BigNumbersTests
    ${BIGINT_TEST_SOURCES}
    ${TEST_HELPER_SOURCES}
)

# ============================================================================
# Target configuration
# ============================================================================

target_link_libraries(BigNumbersTests
    PRIVATE
        BigNumbers::BigNumbers
        GTest::gtest_main
)

# Add sanitizers in debug mode for better error detection
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(BigNumbersTests PRIVATE
            -fsanitize=address,undefined
        )
        target_link_options(BigNumbersTests PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endif()

# ============================================================================
# Test discovery
# ============================================================================

include(GoogleTest)
gtest_discover_tests(BigNumbersTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES
        LABELS "unit"
)
