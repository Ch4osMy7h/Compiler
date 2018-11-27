//
// Created by chaosmyth on 2018/11/21.
//

#include "SymbolTable.h"


void SymbolTable::entry(string n, int type, Catgory c, int addr) {
    SymbolEntry se;
    se.name = n;
    se.type = type;
    se.cat = c;
    se.addr = addr;
    tokenTable.push_back(se);
    index[se] = static_cast<int>(tokenTable.size());
}

void SymbolTable::print() {
    cout << "====================================" << endl;
    cout << "             SymbolTable            " << endl;
    cout << "====================================" << endl;
    cout << "ID\tName\tType\tCat\tAddr" << endl;
    for(int i = 0; i < tokenTable.size(); ++i) {
        cout << i << "\t" << tokenTable[i].name << "\t" << tokenTable[i].type << "\t" << tokenTable[i].cat << "\t" << tokenTable[i].addr << endl;
    }
}

bool operator<(const SymbolEntry &ls, const SymbolEntry &rs) {
    return ls.name < rs.name;
}


