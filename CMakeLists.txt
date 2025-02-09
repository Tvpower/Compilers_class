cmake_minimum_required(VERSION 3.21)
project(compilersAssigment1)

set(CMAKE_CXX_STANDARD 20)

add_executable(compilersAssigment1
        main.cpp
        classes/Lexer.cpp
        classes/Lexer.h
        )
target_include_directories(compilersAssigment1 PRIVATE classes)
