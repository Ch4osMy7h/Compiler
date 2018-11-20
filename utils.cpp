//
// Created by chaosmyth on 2018/11/18.
//

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