//
// Created by chaosmyth on 2018/11/18.
//


#include "Automatic.h"

void Automatic::init() {
    // state 1
    fillChars(1, 2);
    fillNum(1, 3);
    fillState(1, '\'', 9);
    fillState(1, '\"', 11);
    fillState(1, '>', 4);
    fillState(1, '<', 5);
    fillState(1, '=', 6);
    fillDelimter(1, 18);
    fillState(1, '+', 21);
    fillState(1, '-', 24);
    fillState(1, '&', 27);
    fillState(1, '|', 30);
    fillState(1, '^', 33);
    fillState(1, '!', 35);
    fillState(1, '%', 37);
    fillState(1, '*', 39);
    fillState(1, '/', 41);
    // state 2
    fillChars(2, 2);
    fillNum(2, 2);
    fillAll(2, 0);
    // state 3
    fillNum(3, 3);
    fillAll(3, 0);
    // state 4
    fillState(4, '>', 19);
    fillState(4, '=', 13);
    fillDelimter(4, 0);
    //state 5
}

void Automatic::fillChars(int from, int to) {
    for(char val = 'a'; val <= 'z'; val++) {
        ac[from][val] = to;
    }
    for(char val = 'A'; val <= 'Z'; val++) {
        ac[from][val] = to;
    }
}

void Automatic::fillNum(int from, int to) {
    for(char val = '0'; val <= '9'; val++) {
        ac[from][val] = to;
    }
}

void Automatic::fillState(int from, char val, int to) {
    ac[from][val] = to;
}

void Automatic::fillDelimter(int from, int to) {
    //加入了\n', '\t', 空格
    vector<char> delimter = {'+', '-', '*', ',', '.', '(', ')', '[', ']', '{', '}', ';', '\n', '\t', ' '};
    for(auto c : delimter) {
        ac[from][c] = to;
    }
}

void Automatic::fillAll(int from, int to) {
    for(int j = 0; j < 256; ++j) {
        //避免出现覆盖问题
        if (ac[from].count(char(j)) != 0) continue;
        ac[from][char(j)] = to;
    }
}

