cmake_minimum_required(VERSION 3.28)

project(arbys-bignum
    VERSION 0.1.0
    DESCRIPTION "Arbitrary-Precision Arithmetic Library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Only build tests if this is the main project
if(PROJECT_IS_TOP_LEVEL)
    option(ARBYS_BIGNUM_BUILD_TESTS "Build arbys-bignum tests" ON)
else()
    option(ARBYS_BIGNUM_BUILD_TESTS "Build arbys-bignum tests" OFF)
endif()

set(ARBYS_BIGNUM_SOURCES
        src/arbys/bignum/big_int/big_int.cpp
)

set(ARBYS_BIGNUM_DETAIL_SOURCES
        src/arbys/bignum/detail/parse_limbs.cpp
        src/arbys/bignum/detail/normalize.cpp
        src/arbys/bignum/detail/cmp_abs.cpp
        src/arbys/bignum/detail/add_abs.cpp
        src/arbys/bignum/detail/sub_abs.cpp
        src/arbys/bignum/detail/mul_abs.cpp
        src/arbys/bignum/detail/div_abs.cpp
)

add_library(arbys-bignum
    ${ARBYS_BIGNUM_SOURCES}
    ${ARBYS_BIGNUM_DETAIL_SOURCES}
)

add_library(arbys::bignum ALIAS arbys-bignum)

target_include_directories(arbys-bignum
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(arbys-bignum PUBLIC cxx_std_23)

set_target_properties(arbys-bignum PROPERTIES
        EXPORT_NAME bignum
        POSITION_INDEPENDENT_CODE ON
)

# Add compiler warnings
target_compile_options(arbys-bignum PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if(ARBYS_BIGNUM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/arbys-bignum-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

include(GNUInstallDirs)

install(TARGETS arbys-bignum
    EXPORT arbys-bignum-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT arbys-bignum-targets
    FILE arbys-bignum-targets.cmake
    NAMESPACE arbys::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arbys-bignum
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/arbys-bignum-config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/arbys-bignum-config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arbys-bignum
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/arbys-bignum-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/arbys-bignum-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arbys-bignum
)
