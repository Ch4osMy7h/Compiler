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
    fillAll(3, 0);
    fillNum(3, 3);
    fillState(3, '.', 8);
    // state 4
    fillState(4, '>', 19);
    fillState(4, '=', 13);
    fillDelimter(4, 0);
    //state 5
    fillState(5, '<', 20);
    fillState(5, '=', 14);
    fillDelimter(5, 0);
    //state 6
    fillAll(6, 0);
    fillState(6, '=', 15);
    //state 7
    fillAll(7, 0);
    fillNum(7, 7);
    //state 8
    fillNum(8, 7);
    fillDelimter(8, 0);
    //state 9
    fillChars(9, 10);
    // state 10
    fillState(10, '\'', 16);
    // state 11
    fillChars(11, 12);
    // state 12
    fillChars(12, 12);
    fillState(12, '\"', 17);
    // state 13
    fillAll(13, 0);
    // state 14
    fillAll(14, 0);
    // state 15
    fillAll(15, 0);
    // state 16
    fillAll(16, 0);
    // state 17
    fillAll(17, 0);
    // state 18
    fillAll(18, 0);
    // state 19
    fillAll(19, 0);
    //state 20
    fillAll(20, 0);
    // state 21
    fillState(21, '+', 22);
    fillState(21, '=', 23);
    // state 22
    fillAll(22, 0);
    // state 23
    fillAll(23, 0);
    // state 24
    fillState(24, '-', 25);
    fillState(24, '=', 26);
    // state 25
    fillAll(25, 0);
    // state 26
    fillAll(26, 0);
    // state 27
    fillAll(27, 0);
    fillState(27, '=', 28);
    fillState(27, '&', 29);
    // state 28
    fillAll(28, 0);
    // state 29
    fillAll(29, 0);
    // state 30
    fillAll(30, 0);
    fillState(30, '=', 31);
    fillState(30, '|', 32);
    // state 31
    fillAll(31, 0);
    // state 32
    fillAll(32, 0);
    // state 33
    fillAll(33, 0);
    fillState(33, '=', 34);
    // state 34
    fillAll(34, 0);
    // state 35
    fillAll(35, 0);
    fillState(35, '=', 36);
    // state 36
    fillAll(36, 0);
    // state 37
    fillAll(37, 0);
    fillState(37, '=', 38);
    // state 38
    fillAll(38, 0);
    // state 39
    fillAll(39, 0);
    fillState(39, '=', 40);
    // state 40
    fillAll(40, 0);
    // state 41
    fillAll(41, 0);
    fillState(41, '=', 42);
    // state 42
    fillAll(42, 0);

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
    vector<char> delimter = {'+', '-', '*', ',', '.', '(', ')', '[', ']', '{', '}', ';'};
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

Automatic::Automatic() {
    init();
}

int Automatic::getState(int state, char c) {
    return ac[state][c];
}

