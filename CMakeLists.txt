cmake_minimum_required(VERSION 3.21)
project(processtree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(spdlog REQUIRED)

add_executable(
        processtree
        main.cpp
        parsing.cpp
        io_stdio.cpp
)

target_link_libraries(
        processtree
        spdlog::spdlog
)

# https://cmake.org/cmake/help/latest/module/FindGTest.html
find_package(GTest REQUIRED)

enable_testing()

add_executable(
        simple_test
        processtree_tests.cpp
)
target_link_libraries(
        simple_test
        ${GTEST_MAIN_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(simple_test)
