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

KeyWordTable::KeyWordTable() {
    vector<string> keyWord = {"int", "main", "void", "if",
                      "else", "char", "double", "float",
                      "while", "for", "do", "break", "struct",
                      "long", "bool", "return", "function", "use"};
    for(auto c : keyWord) {
        tokenTable.push_back(c);
        index[c] = static_cast<int>(tokenTable.size());
    }
}


class IdentiferTable : public Table<string> {
    IdentiferTable() = default;
};



class DelimiterTable : public Table<string> {
    DelimiterTable();
};

DelimiterTable::DelimiterTable() {
    vector<string> delimter = {"+", "-", "*", ",", ".", "(", ")", "[", "]", "{", "}", ";",
                         ">>", ">=", "<<", "<=", "<", "==", "=" ,"++", "+=", "--", "-=",
                         "&=", "&&", "&", "|=", "||", "|", "^", "^=", "!", "!=", "%",
                         "%=", "*", "*=", "/", "/="};
    for(auto ch : delimter) {
        tokenTable.push_back(ch);
        index[ch] = static_cast<int>(tokenTable.size());
    }
}

class IntTable : public Table<int> {
    IntTable() = default;
};

class FloatTable : public Table<int> {
    FloatTable() = default;
};

class CharTable : public Table<int> {
    CharTable() = default;
};

class StringTable : public  Table<string> {
    StringTable() = default;
};



#endif //COMPILER_TYPETABLE_H
