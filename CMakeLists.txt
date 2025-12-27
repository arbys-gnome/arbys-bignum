cmake_minimum_required(VERSION 3.28)

project(BigNumbers
    VERSION 1.0.0
    DESCRIPTION "Arbitrary precision arithmetic library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Only build tests if this is the main project
if(PROJECT_IS_TOP_LEVEL)
    option(BIGNUMBERS_BUILD_TESTS "Build BigNumbers tests" ON)
else()
    option(BIGNUMBERS_BUILD_TESTS "Build BigNumbers tests" OFF)
endif()

set(BIGINT_SOURCES
        src/arbys/bignumbers/bigint/big_int.cpp
)

set(DETAIL_SOURCES
        src/arbys/bignumbers/detail/parse_limbs.cpp
        src/arbys/bignumbers/detail/normalize.cpp
        src/arbys/bignumbers/detail/cmp_abs.cpp
        src/arbys/bignumbers/detail/add_abs.cpp
        src/arbys/bignumbers/detail/sub_abs.cpp
        src/arbys/bignumbers/detail/mul_abs.cpp
        src/arbys/bignumbers/detail/config.h
        src/arbys/bignumbers/detail/div_abs.cpp
        src/arbys/bignumbers/detail/big_int_internal.h
        include/arbys/bignumbers/results.h
)

add_library(BigNumbers
    ${BIGINT_SOURCES}
    ${DETAIL_SOURCES}
)

# Add an alias for use when included as a subdirectory
add_library(BigNumbers::BigNumbers ALIAS BigNumbers)

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

if(BIGNUMBERS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

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
