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
    bool name1Act;
    bool name2Act;
    bool resAct;
    QuadTuple(string o,  string n1, string n2, string r) {
        op = o;
        name1 = n1;
        name2 = n2;
        res = r;

    }
    void print() {
        cout << setw(8) << op << setw(8) << name1 << setw(8) << name2 << setw(8) << res << endl;
    }
    void printWithAct(ofstream& out) {
        out << setw(8) << op << setw(8) << name1 << "(" << name1Act  << ")" << setw(8) << name2 << "(" << name2Act  << ")"
        << setw(8) << res << "(" << resAct  << ")" << endl;
    }


};




#endif //COMPILER_QUADTURPLE_H
