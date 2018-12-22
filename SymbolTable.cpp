//
// Created by chaosmyth on 2018/11/21.
//

#include "SymbolTable.h"




void SymbolTable::print() {
    cout << "====================================" << endl;
    cout << "             SymbolTable            " << endl;
    cout << "====================================" << endl;
    cout << "ID\tName\tType\tCat\tAddr" << endl;
}



int SymbolTable::searchSymbolName(string name, int curInd) {
    for(int i = 0; i < symbolTable[curInd].size(); i++) {
        if(symbolTable[curInd][i].name == name) {
            return i;
        }
    }
    return -1;
}

bool operator<(const SymbolTableElement &ls, SymbolTableElement &rs) {
    return ls.name < rs.name;
}

Type SymbolTable::searchSymbolType(string name,int curFun) {
    //先搜索当前作用域
    for (auto &i : symbolTable[curFun]) {
        if(i.name == name) {
            return i.type;
        }
    }
    for (auto &i : symbolTable[0]) {
        if(i.name == name) {
            return i.type;
        }
    }


    if(name.find('[') != string::npos) {
        int index = 0;
        while(name[index] != '[') index++;
        name = name.substr(0, index);
        for (auto &i : symbolTable[curFun]) {
            if(i.name == name) {
                int index = i.aiInd;
                return arrayTableVec[index].type;
            }
        }
        for (auto &i : symbolTable[0]) {
            if(i.name == name) {
                return i.type;
            }
        }
    }

    //形式参数里面搜索
    if(curFun != 0) {
        int index = symbolTable[curFun][0].pfinalind;
        for(auto &i : functionTableVec[index].pi) {
            if(i.name == name) {
                return i.type;
            }
        }
    }

    //cout << name << " " << curFun << endl;
    cout << "该变量" << name << "未定义" << endl;
    exit(0);
}



bool SymbolTable::isTempName(string name, string curFun) {
    int index = funToName[curFun];
    for (auto &i : symbolTable[index]) {
        if(i.name == name && i.isTemp) {
            return true;
        }
    }
//    cout << "未找到该变量" << endl;
    return false;
}

bool SymbolTable::isActive(string name, string curFun) {
    int index = funToName[curFun];
    for (auto &i : symbolTable[index]) {
        if(i.name == name) {
            return i.isActive;
        }
    }
    for (auto &i : symbolTable[0]) {
        if(i.name == name) {
            return i.isActive;
        }
    }

//    cout << "未找到该变量" << endl;
    return false;
}

void SymbolTable::setActive(string name, string curFun, bool active) {
    int index = funToName[curFun];
    for (auto &i : symbolTable[index]) {
        if(i.name == name) {
            i.isActive = active;
        }
    }
    for (auto &i : symbolTable[0]) {
        if(i.name == name) {
            i.isActive = active;
        }
    }
}


pair<bool, int> SymbolTable::getAddr(string name, string curFun) {
    int index = funToName[curFun];
    pair<bool , int> res;   // bool 代表是否全局区， int代表相对地址
    for(auto &i : symbolTable[index]) {
        if(i.name == name) {
            res.first = false;
            res.second = i.vall;
            return res;
        }
    }
    for(auto &i : symbolTable[0]) {
        if(i.name == name) {
            res.first = true;
            res.second = i.vall;
            return res;
        }
    }
    return make_pair(false, -1);
}

int SymbolTable::getFuncSize(string curFun)
{
    return curFun.empty() ? vallVec[0] : vallVec[funToName[curFun]];
}

bool SymbolTable::getSymbolTableFuncName(string name) {
    for(auto& i : symbolTable) {
        if(i[0].name == name) {
            return true;
        }
    }
    return false;
}
