//
// Created by chaosmyth on 2018/11/22.
//

#ifndef COMPILER_QUADTURPLE_H
#define COMPILER_QUADTURPLE_H


#include "bits/stdc++.h"

using namespace std;



class QuadTurple {
public:
    QuadTurple(const string& op, const string &name1, const string &name2, const string &res);

private:
    string op;
    string name1;
    string name2;
    string res;

};




#endif //COMPILER_QUADTURPLE_H
