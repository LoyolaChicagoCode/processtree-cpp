cmake_minimum_required(VERSION 3.21)
project(processtree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        8.1.1
)
FetchContent_MakeAvailable(fmt)

# Lots of compilation errors without this setting
option(SPDLOG_FMT_EXTERNAL "Use external fmt library instead of bundled" ON)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.9.2
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        scnlib
        GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
        GIT_TAG        v0.4
)
FetchContent_MakeAvailable(scnlib)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.1.2
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG        v0.7.3
)
FetchContent_MakeAvailable(magic_enum)

add_executable(
        processtree
        main.cpp
        processtree.cpp
)

target_link_libraries(
        processtree
        scn::scn
        fmt::fmt
        spdlog::spdlog
        CLI11::CLI11
        magic_enum
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
        processtree.cpp
)
target_link_libraries(
        processtree_tests
        gtest_main
        scn::scn
        fmt::fmt
        spdlog::spdlog
        CLI11::CLI11
        magic_enum
)

include(GoogleTest)
gtest_discover_tests(processtree_tests)
