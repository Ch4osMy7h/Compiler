//
// Created by chaosmyth on 2018/11/18.
//
#ifndef COMPILER_MYUTILS_H
#define COMPILER_MYUTILS_H
#include <bits/stdc++.h>
#include <sstream>
using namespace std;

template <typename T>
T string2num(string str) {
    istringstream is(str);
    T num;
    is >> num;
    return num;
}
bool isNum(string str);
bool isOperator(string str);
string calculateNum(string op, string a, string b);
#endif //COMPILER_MYUTILS_H
