cmake_minimum_required(VERSION 3.3)
project(C_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(HEAD_FILES include/io.h)

set(SOURCE_FILES src/main.cpp)

add_executable(bin/SeedCup2016 ${SOURCE_FILES} ${HEAD_FILES})
