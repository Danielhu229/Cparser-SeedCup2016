cmake_minimum_required(VERSION 3.3)
project(C_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(googletest/googletest)

include_directories(../include unit)

set(C_PARSER_HEAD
        ${gtest_SOURCE_DIR}
        ../include
        )

set(C_PARSER_SRC
        unit/test_main.cpp
        unit/ParserTest.cpp
        unit/LexerTest.cpp
        unit/IoTest.cpp
        unit/InterpreterTest.cpp
        ../src/Parser.cpp
        ../src/Token.cpp
        ../src/Lexer.cpp
        ../src/Utility.cpp
        ../src/Interpreter.cpp
        )

add_executable(SeedCup2016_Test ${C_PARSER_SRC} ${C_PARSER_HEAD})

target_link_libraries(SeedCup2016_Test gtest)