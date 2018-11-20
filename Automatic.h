//
// Created by chaosmyth on 2018/11/18.
//

#ifndef COMPILER_AUTOMATIC_H
#define COMPILER_AUTOMATIC_H


#include <bits/stdc++.h>
using namespace std;
const int stateNum = 70;

class Automatic {
private:
    map<char, int> ac[stateNum];
public:
    Automatic();
    void init();
    void fillChars(int from, int to);
    void fillNum(int from, int to);
    void fillState(int from, char val, int to);
    void fillDelimter(int from, int to);
    void fillAll(int from, int to);
    int getState(int state, char c);
};



#endif //COMPILER_AUTOMATIC_H
