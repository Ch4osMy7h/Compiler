//
// Created by chaosmyth on 2018/11/21.
//

#include "Parse.h"
using namespace std;

Parse::Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
    this->curFun = -1;
    this->funCnt = 0;
}

int Parse::program() {
    int index = curIndex;
    //fun语义动作
    QuadTurple qt("fun", "-1", "*", "*");
    quadVec.push_back(qt);
    if(declarationList()) {
        QuadTurple qt("funend", "-1", "*", "*");
        quadVec.push_back(qt);
        return 1;
    }
    curIndex = index;
    return 0;
}


int Parse::declarationList() {
    if(curIndex == tokenVec.size()) {
        return 0;
    }
    int index = curIndex;
    int fun = curFun;
    int funC = funCnt;
    if(!declaration()) {
        curIndex = index;
        curIndex = index;
        curFun = fun;
        funCnt = funC;
        return 0;
    }
    declaration();
    return 1;
}

int Parse::declaration() {
    //是否有必要回溯
    if(funDeclaration()) return 1;
    if(varDeclaration()) return 1;
    return 0;
}

int Parse::varDeclaration() {
    int index = curIndex;
    if(typeSpecifier()) {
        if(tokenVec[curIndex].type != TokenType::IDENTIFIER) {
            cout << "Error occured when missing variable name or function name" << endl;
            exit(0);
        }
        string curName = tokenVec[curIndex].name;
        TokenType curType = tokenVec[curIndex].type;
        if(curFun == -1) {
            int stIndex = 0;
            if(stIndex = st.searchSymbolName(tokenVec[curIndex].name)) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }
        } else {
            int stIndex = 0;
            //符号表怎么判断作用域 ???
            if(stIndex = st.searchSymbolName(tokenVec[curIndex].name)) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }

        }
        bool arrayFlag = false;
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type ==  INTCONST) {
                curIndex++;
            } else {
                cout << "error array size" << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
                arrayFlag = true;
            } else {
                cout << "miss an ] flag when defina an array" << endl;
                exit(0);
            }
        } else if(tokenVec[curIndex].type == IDENTIFIER) {
            curIndex++;
        } else {
            cout << "error when define variable" << endl;
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "miss an ; flag when define an array" << endl;
        }

        //语义动作
        if(arrayFlag) {
            quadVec.emplace_back(curName, "arr", "0", "##");
        } else {
            quadVec.emplace_back(curName, to_string(curType), "0", "##");
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::typeSpecifier() {
    if(tokenVec[curIndex].type == KEYWORD && isType(tokenVec[curIndex])) {
        curIndex++;
        return 1;
    } else {
        cout << "Uncorreted type specifier" << endl;
        return 0;
    }
}

bool Parse::isType(Token token) {
    return token.type == KEYWORD && (token.id == keyWordTable.index["int"] || token.id == keyWordTable.index["double"] || token.id == keyWordTable.index["void"] || token.id == keyWordTable.index["char"]);
}

int Parse::funDeclaration() {
    if(typeSpecifier()) {
        string curName;
        if(tokenVec[curIndex].type == IDENTIFIER) {
            curIndex++;
            curName = tokenVec[curIndex].name;
        } else {
            cout << "Error occured when missing variable name or function name" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "Error occured when missing \"(\" char" << endl;
            exit(0);
        }
        params();
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "Error occured when missing \")\" char" << endl;
            exit(0);
        }
        if(compoundStmt(false)) {
            //语义动作 函数四元式
            return 1;
        } else {
            cout << "函数定义失败" << endl;
        }
    }
    return 0;
}

int Parse::params() {
    int index = curIndex;
    if (paramList()) {
        return 1;
    }
    curIndex = index;
    return 1;
}

int Parse::paramList() {
    int index = curIndex;
    if(!param()) {
        curIndex = index;
        return 0;
    }
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id) {
        curIndex++;
        if(!paramList()) {
            cout << "Error occured when need a param after\",\"" << endl;
            exit(0);
        }
    }
    return 1;
}

int Parse::param() {
    int index = curIndex;
    if(typeSpecifier()) {
        string curName;
        if(tokenVec[curIndex].type == IDENTIFIER) {
            curName = tokenVec[curIndex].name;
            curIndex++;
        } else {
            cout << "Error occured when need an identifer after type" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
            } else {
                cout << "Error occured when missing \"]\" char" << endl;
                exit(0);
            }
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::compoundStmt(bool isOutput) {
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["{"] == tokenVec[curIndex].id) {
        curIndex++;
    }
    return 0;
}









