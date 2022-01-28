cmake_minimum_required(VERSION 3.21)
project(processtree)

set(CMAKE_CXX_STANDARD 20)

add_executable(processtree main.cpp parsing.cpp io_iostream.cpp)
