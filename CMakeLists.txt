cmake_minimum_required(VERSION 3.22)

project(apotheosis CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(default-options.cmake)

add_executable(a.out main.cpp graph-getter.cpp)
