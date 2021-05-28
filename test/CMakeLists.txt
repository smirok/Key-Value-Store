cmake_minimum_required(VERSION 3.15)
project(tests)

set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${TEST_DIR})

cmake_minimum_required(VERSION 3.19)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests main.cpp)
target_link_libraries(tests
        PRIVATE "gtest_main"
        PRIVATE "gmock_main")

add_test(
        NAME "tests"
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/tests
)

# Code coverage
if (ENABLE_TEST_COVERAGE)
    set(COVERAGE_LCOV_EXCLUDES "${TEST_DIR}/*" "*/googletest/*" "*/googlemock/*" "/usr/*")
    setup_target_for_coverage_lcov(
            NAME "coverage"         # New target name
            EXECUTABLE "ctest"     # Executable to run to gather coverage stats
            DEPENDENCIES "tests"    # Dependencies to build first
    )
endif()