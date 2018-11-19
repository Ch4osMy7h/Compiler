//
// Created by chaosmyth on 2018/11/18.
//

#include <bits/stdc++.h>
#include "Scanner.h"
#include <cstdio>


void Scanner::scan(string filename) {
    int state = 1;
    int stateBefore;
    char curChar;
    int pos = 0;
    string buffer = "";
    FILE* fp;
    Automatic at;
    //暂时取消
    if( (fp = fopen("src.txt", "r")) == NULL) {
        cout << "open Error" << endl;
    }
    while ((curChar = static_cast<char>(getc(fp))) != '#') {
        stateBefore = state;
        state = at.getState(state, curChar);
        if(state == 1 && stateBefore == 1) {}
        else if(state) {
            buffer += curChar;
        } else {
            tokenGenerate(stateBefore, buffer);
        }
    }
}

void Scanner::tokenGenerate(int state_before, string buffer) {
    string code = encoder(state_before);
    Token token;
    if(code == "k") {
        if(keyWordTable.contains(buffer)) {
            token.id = keyWordTable.getID(buffer);
            token.name = buffer;
            token.type = KEYWORD;
            tokenVec.push_back(token);
        } else {
            identiferTable.addElement(buffer);
            token.id = identiferTable.getID(buffer);
            token.name = buffer;
            token.type = IDENTIFIER;
            tokenVec.push_back(token);
        }
    }
}

string Scanner::encoder(int state) {
    map<int, string> finalState;
    finalState[2] = "k";
    finalState[3] = "int";
    finalState[7] = "float";
    finalState[16] = "char";
    finalState[17] = "string";
    finalState[19] = "p";
    finalState[13] = "p";
    finalState[4]  = "p";
    finalState[20] = "p";
    finalState[14] = "p";
    finalState[5] = "p";
    finalState[6] = "p";
    finalState[15] = "p";
    finalState[18] = "p";
    for(int i = 25; i <= 42; i++)
    finalState[22] = "p";
    finalState[23] = "p";
    for(int i = 25; i <= 42; i++) {
        finalState[25] = "p";
    }
    return finalState[state];
}
