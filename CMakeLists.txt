cmake_minimum_required(VERSION 3.3)
project(C_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(test)

include_directories(include)

set(C_PARSER_HEAD include/)

set(C_PARSER_SRC src/main.cpp)

add_executable(SeedCup2016 ${C_PARSER_SRC} ${C_PARSER_HEAD})
