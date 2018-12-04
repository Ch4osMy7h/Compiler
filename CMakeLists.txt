cmake_minimum_required(VERSION 3.12)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler Token.h  Token.cpp front-end/Automatic.cpp front-end/Automatic.h Scanner.cpp Scanner.h MyUtils.h main.cpp Table.h TypeTable.h TypeTable.cpp SymbolTable.cpp SymbolTable.h front-end/Parse.cpp front-end/Parse.h QuadTuple.cpp QuadTuple.h back-end/DAG.cpp back-end/DAG.h)
