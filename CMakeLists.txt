cmake_minimum_required(VERSION 3.22)
project(matrix)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(matrix
        matrix.cpp
        matrix.hpp
        ex.csv)
