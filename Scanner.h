//
// Created by chaosmyth on 2018/11/18.
//

#ifndef COMPILER_SCANNER_H
#define COMPILER_SCANNER_H

#include <bits/stdc++.h>
#include "Token.h"
#include "Automatic.h"
#include "TypeTable.h"
using namespace std;

class Scanner {
private:
    KeyWordTable keyWordTable;
    IdentiferTable identiferTable;
    DelimiterTable delimiterTable;
    IntTable intTable;
    FloatTable floatTable;
    CharTable charTable;
    StringTable stringTable;
    vector<Token> tokenVec;
public:
    Scanner() = default;
    void scan(string filename);
    void tokenGenerate(int state_before, string buffer);
    string encoder(int state);
};




#endif //COMPILER_SCANNER_H
