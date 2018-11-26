cmake_minimum_required(VERSION 3.12)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler Token.h  Token.cpp  Automatic.cpp Automatic.h Scanner.cpp Scanner.h utils.cpp main.cpp Table.h TypeTable.h TypeTable.cpp SymbolTable.cpp SymbolTable.h Parse.cpp Parse.h QuadTurple.cpp QuadTurple.h)
