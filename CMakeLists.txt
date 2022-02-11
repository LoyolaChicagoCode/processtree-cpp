cmake_minimum_required(VERSION 3.21)
project(processtree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(spdlog REQUIRED)

find_package(fmt REQUIRED)

include(FetchContent)
FetchContent_Declare(
        scnlib
        GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
        GIT_TAG        v0.4
)
FetchContent_MakeAvailable(scnlib)

add_executable(
        processtree
        main.cpp
        parsing.cpp
)

target_link_libraries(
        processtree
        spdlog::spdlog
        fmt::fmt
        scn::scn
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        processtree_tests
        processtree_tests.cpp
        parsing.cpp
)
target_link_libraries(
        processtree_tests
        gtest_main
        spdlog::spdlog
        fmt::fmt
        scn::scn
)

include(GoogleTest)
gtest_discover_tests(processtree_tests)
