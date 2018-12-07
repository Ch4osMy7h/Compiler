//
// Created by chaosmyth on 2018/11/22.
//

#ifndef COMPILER_QUADTURPLE_H
#define COMPILER_QUADTURPLE_H


#include "bits/stdc++.h"

using namespace std;


class QuadTuple {
public:
    string op;
    string name1;
    string name2;
    string res;
    QuadTuple(string o,  string n1, string n2, string r) {
        op = o;
        name1 = n1;
        name2 = n2;
        res = r;
    }

};




#endif //COMPILER_QUADTURPLE_H
