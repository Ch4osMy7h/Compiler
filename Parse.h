//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_PARSE_H
#define COMPILER_PARSE_H

#include "bits/stdc++.h"
#include "QuadTurple.h"
#include "Token.h"
#include "SymbolTable.h"
#include "TypeTable.h"

using namespace std;



class Parse {
private:
    vector<QuadTurple>& quadVec;
    QuadTurple quadTurple;
    vector<Token>& tokenVec;
    SymbolTable& st;
    KeyWordTable& keyWordTable;
    IdentiferTable& identiferTable;
    DelimiterTable& delimiterTable;
    IntTable& intTable;
    FloatTable& floatTable;
    CharTable& charTable;
    StringTable& stringTable;
    stack<Token> sem;
    int curIndex;
public:
    Parse();

    Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
              IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
              StringTable &stTable);

    void START();
    void S();
    void Function();
    void FunctionSon();
    void FuncitionDataType();
    void Struct();
    void Mainfunc();

};



#endif //COMPILER_PARSE_H
