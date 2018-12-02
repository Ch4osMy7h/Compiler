//
// Created by chaosmyth on 2018/11/18.
//

#ifndef COMPILER_SCANNER_H
#define COMPILER_SCANNER_H

#include <bits/stdc++.h>
#include "Token.h"
#include "TypeTable.h"
#include "front-end/Automatic.h"
#include "MyUtils.h"
#include "SymbolTable.h"
using namespace std;

class Scanner {
private:
    KeyWordTable& keyWordTable;
    IdentiferTable& identiferTable;
    DelimiterTable& delimiterTable;
    IntTable& intTable;
    FloatTable& floatTable;
    CharTable& charTable;
    StringTable& stringTable;
    vector<Token>& tokenVec;
    SymbolTable& symbolTable;

public:
    Scanner(KeyWordTable & kt, IdentiferTable& it, DelimiterTable& dt, IntTable & inta, FloatTable& ft, CharTable& ct, StringTable& st, vector<Token>& tokenVec, SymbolTable& syt);
    void preProcess();
    void scan(string filename);
    void tokenGenerate(int state_before, string buffer);
    string encoder(int state);
    void reset(FILE* fp, int& state, int& pos, string& buffer);
    void showLex();
    string numToName(int num);
};




#endif //COMPILER_SCANNER_H
