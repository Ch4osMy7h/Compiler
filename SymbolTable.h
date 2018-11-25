//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include "Table.h"

using namespace std;


enum Catgory {
    Array = 0,
    Struct = 1,
    Variable = 2,
    None = 3
};

struct SymbolEntry {
    string name;
    int type;
    Catgory cat;
    int addr;
    //bool isActive;
};

class SymbolTable : public Table<SymbolEntry> {
public:
    void entry(string n, int type, Catgory c, int addr);
    void print();
    friend bool operator<(const SymbolEntry &ls, const SymbolEntry &rs);
};









#endif //COMPILER_SYMBOLTABLE_H
