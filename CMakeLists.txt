cmake_minimum_required(VERSION 3.21)
project(build/compilersAssigment2)

set(CMAKE_CXX_STANDARD 20)

add_executable(compilersAssigment2
        main.cpp
        classes/Lexer.cpp
        classes/Lexer.h
        classes/parser.cpp
        classes/parser.h
        )
target_include_directories(compilersAssigment2 PRIVATE classes)
