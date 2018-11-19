//
// Created by chaosmyth on 2018/11/19.
//

#ifndef COMPILER_TABLE_H
#define COMPILER_TABLE_H
/*
 * 所有表(不包括符号表系统)
 */

#include <bits/stdc++.h>
#include <cstring>
using namespace std;

template <class T>
class Table {
public:
    vector<T> tokenTable;
    map<T, int> index; //提高索引速度
    void print(string tableName);
    void addElement(T ele);
    int getID(T v);
    bool contains(T v);
};

template<class T>
void Table<T>::print(string tableName) {
    cout << "====================================" << endl;
    cout << "        " << tableName << "              " << endl;
    cout << "====================================" << endl;
    cout << "ID\tValue" << endl;
    auto it = tokenTable.begin();
    for(size_t i = 0; i < tokenTable.size(); i++) {
        cout << i << "\t" << *it << endl;
    }
}

template<class T>
bool Table<T>::contains(T v) {
    return index.count(v) != 0;
}

template<class T>
int Table<T>::getID(T v) {
    return index[v];
}

template<class T>
void Table<T>::addElement(T ele) {
    tokenTable.push_back(ele);
    index[ele] = tokenTable.size();
}


#endif //COMPILER_TABLE_H
