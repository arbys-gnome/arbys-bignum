cmake_minimum_required(VERSION 3.28)

project(BigNumbers
    VERSION 1.0.0
    DESCRIPTION "Arbitrary precision arithmetic library"
    LANGUAGES CXX
)

# ============================================================================
# Project-wide settings
# ============================================================================

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Only build tests if this is the main project
if(PROJECT_IS_TOP_LEVEL)
    option(BIGNUMBERS_BUILD_TESTS "Build BigNumbers tests" ON)
else()
    option(BIGNUMBERS_BUILD_TESTS "Build BigNumbers tests" OFF)
endif()

# ============================================================================
# Library target
# ============================================================================

set(BIGINT_SOURCES
    src/bigint/bigint.cpp
    src/bigint/add.cpp
    src/bigint/sub.cpp
    src/bigint/mul.cpp
    src/bigint/div.cpp
    src/bigint/cmp.cpp
    src/bigint/from_string.cpp
)

set(DETAIL_SOURCES
        src/detail/parse_digits.cpp
        src/detail/normalize.cpp
        src/detail/cmp_abs.cpp
        src/detail/add_abs.cpp
        src/detail/sub_abs.cpp
        src/detail/mul_abs.cpp
        src/detail/bigint_impl.h
)

add_library(BigNumbers
    ${BIGINT_SOURCES}
    ${DETAIL_SOURCES}
)

# Add an alias for use when included as a subdirectory
add_library(BigNumbers::BigNumbers ALIAS BigNumbers)

# ============================================================================
# Target properties
# ============================================================================

target_include_directories(BigNumbers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(BigNumbers PUBLIC cxx_std_26)

# Add compiler warnings
target_compile_options(BigNumbers PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# ============================================================================
# Testing
# ============================================================================

if(BIGNUMBERS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Installation
# ============================================================================

include(GNUInstallDirs)

install(TARGETS BigNumbers
    EXPORT BigNumbersTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT BigNumbersTargets
    FILE BigNumbersTargets.cmake
    NAMESPACE BigNumbers::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BigNumbers
)
