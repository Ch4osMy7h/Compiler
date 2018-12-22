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
bool isTempName(string name, string curFun);
bool isActive(string name,string curFun); //判断是否活跃
void setActive(string name, string curFun, bool active); //设置活跃信息
pair<bool, int> getAddrFromTable(string name, string curFun);
int getFuncSize(string curFun);
bool isGlobalName(string name, string curFun);

string calculateNum(string op, string a, string b);

#endif //COMPILER_MYUTILS_H
