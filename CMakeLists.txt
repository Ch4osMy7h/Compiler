cmake_minimum_required(VERSION 3.12)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(back-end)
include_directories(front-end)

add_executable(Compiler
        back-end/DAG.cpp
        back-end/DAG.h
        front-end/Automatic.cpp
        front-end/Automatic.h
        front-end/Parse.cpp
        front-end/Parse.h
        lex.txt
        main.cpp
        MyUtils.h
        QuadTuple.h
        Scanner.cpp
        Scanner.h
        src.txt
        SymbolTable.cpp
        SymbolTable.h
        Table.h
        Token.cpp
        Token.h
        TypeTable.cpp
        TypeTable.h MyUtils.cpp back-end/BasicBlock.cpp back-end/BasicBlock.h back-end/Assembly.cpp back-end/Assembly.h)
