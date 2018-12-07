//
// Created by chaosmyth on 2018/11/21.
//

#include "SymbolTable.h"




void SymbolTable::print() {
    cout << "====================================" << endl;
    cout << "             SymbolTable            " << endl;
    cout << "====================================" << endl;
    cout << "ID\tName\tType\tCat\tAddr" << endl;
    for(int i = 0; i < tokenTable.size(); ++i) {
        cout << i << "\t" << tokenTable[i].name << "\t" << tokenTable[i].type << "\t" << tokenTable[i].cat << "\t" << tokenTable[i].addr << endl;
    }
}



int SymbolTable::searchSymbolName(string name) {
    for(int i = 0; i < tokenTable.size(); i++) {
        if(tokenTable[i].name == name) {
            return i;
        }
    }
    return -1;
}

bool operator<(const SymbolTableElement &ls, SymbolTableElement &rs) {
    return ls.name < rs.name;
}


