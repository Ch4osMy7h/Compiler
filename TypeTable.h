//
// Created by chaosmyth on 2018/11/19.
//

#ifndef COMPILER_TYPETABLE_H
#define COMPILER_TYPETABLE_H

#include "Table.h"
#include <bits/stdc++.h>
using namespace std;

class KeyWordTable : public Table<string> {
public:
    KeyWordTable();
};



class IdentiferTable : public Table<string> {
public:
    IdentiferTable() = default;
};



class DelimiterTable : public Table<string> {
public:
    DelimiterTable();
};


class IntTable : public Table<int> {
public:
    IntTable() = default;
};

class FloatTable : public Table<double > {
public:
    FloatTable() = default;
};

class CharTable : public Table<char > {
public:
    CharTable() = default;
};

class StringTable : public  Table<string > {
public:
    StringTable() = default;
};



#endif //COMPILER_TYPETABLE_H
