//
// Created by chaosmyth on 2018/11/18.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <bits/stdc++.h>

using namespace std;
enum TokenType
{
    IDENTIFIER = 1,
    INTCONST = 2,
    FLOATCONST = 3,
    DELIMTER = 4,
    KEYWORD = 5,
    CHARCONST = 6,
    STRCONST = 7,
    OPERATOR = 8
};



class Token {
public:
    TokenType type;
    string name;
    int id;
    Token();
    Token(TokenType type, string name, int id);
    void print();
};
#endif //COMPILER_TOKEN_H
