//
// Created by chaosmyth on 2018/11/21.
//

#include "Parse.h"


Parse::Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
}

void Parse::START() {
    curIndex++;
    S();
    if(curIndex >=tokenVec.size()) {
        cout << "程序没有正常结束" << endl;
        return ;
    }
    cout << "程序运行成功" << endl;
}

void Parse::S() {
    Function();
    Struct();
    Mainfunc();
}

void Parse::Function() {
    cout << "函数递归子程序" << endl;
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["function"]) {
        curIndex++;
        FunctionSon();
    }
}



void Parse::Struct() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["struct"]) {
        curIndex++;
        if(tokenVec[curIndex].id ) {}
    }
}


void Parse::FunctionSon() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"]))  {
        FuncitionDataType();
    }
}

void Parse::FuncitionDataType() {
    cout << "entry FunctionDatatype" << endl;
    if()  {
        FuncitionDataType();
    }
}






