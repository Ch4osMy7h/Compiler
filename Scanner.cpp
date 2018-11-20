//
// Created by chaosmyth on 2018/11/18.
//

#include "Scanner.h"


void Scanner::scan(string filename) {
    int state = 1;
    int stateBefore;
    char curChar;
    int pos = 0;
    string buffer = "";
    FILE* fp;
    Automatic at;
    //暂时取消
    if( (fp = fopen("../src.txt", "r")) == NULL) {
        cout << "open Error" << endl;
    }
    while ((curChar = static_cast<char>(getc(fp))) != '#') {
        cout << curChar << endl;
        if(curChar == '\n' || curChar == '\t' || curChar == ' ' || curChar == '\r') {
            tokenGenerate(state, buffer);
            pos = 0;
            buffer = "";
            state = 1;
            continue;
        }
        stateBefore = state;
        state = at.getState(state, curChar);
        if(state == 1 && stateBefore == 1) {}
        else if(state) {
            buffer += curChar;
        } else {
            tokenGenerate(stateBefore, buffer);
            reset(fp, state, pos, buffer);
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
    } else if(code == "int") {
        int num = string2num<int >(buffer);
        intTable.addElement(num);
        token.id = intTable.getID(num);
        token.name = buffer;
        token.type = INTCONST;
        tokenVec.push_back(token);
    } else if(code == "float") {
        auto num = string2num<double >(buffer);
        token.id = floatTable.getID(num);
        token.name = buffer;
        token.type = FLOATCONST;
        tokenVec.push_back(token);
    } else if(code == "char") {
        charTable.addElement(buffer[0]);
        token.id = charTable.getID(buffer[0]);
        token.name = buffer;
        token.type = CHARCONST;
        tokenVec.push_back(token);
    } else if(code == "string") {
        stringTable.addElement(buffer);
        token.id = stringTable.getID(buffer);
        token.name = buffer;
        token.type = STRCONST;
        tokenVec.push_back(token);
    } else if(code == "p") {
        token.id = delimiterTable.getID(buffer);
        token.name = buffer;
        token.type = DELIMTER;
        tokenVec.push_back(token);
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
    finalState[22] = "p";
    finalState[23] = "p";
    finalState[21] = "p";
    finalState[24] = "p";
    for(int i = 25; i <= 42; i++) {
        finalState[25] = "p";
    }
    return finalState[state];
}

void Scanner::reset(FILE *fp, int &state, int &pos, string &buffer) {
    pos = 0;
    buffer = "";
    state = 1;
    fseek(fp, -1L, 1);
}

void Scanner::showLex() {
    freopen("../lex.txt", "w", stdout);
    cout << "\t\t词法分析结果\t\t" << endl;
    for(auto val : tokenVec) {
        cout << "\t "<< val.name << "\t" << numToName(val.type) << "\t" << val.id << endl;
    }
}
string Scanner::numToName(int num) {
    map<int, string> changeTable;
    changeTable[1] = "identifer";
    changeTable[2] = "int";
    changeTable[3] = "float";
    changeTable[4] = "delimter";
    changeTable[5] = "keyword";
    changeTable[6] = "char";
    changeTable[7] = "string";
    return changeTable[num];
}

