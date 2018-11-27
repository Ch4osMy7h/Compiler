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


enum Type {
    ArrayType = 1,
    StructType = 2,
    IntType = 3,
    CharType = 4,
    BoolType = 5,
    FloatType = 6,
    DoubleType = 7
};

struct SymbolEntry {
    string name;
    int type;
    Catgory cat;
    int addr;
    //bool isActive;
};

struct TypeTable {
    Type type;
    int TPoint; // 0 == null , 1 == array, 2 == struct
};

struct ArrayTable {
    int low = 0;
    int up;
    int ctp, clen;
};

struct StructTableVal {
    string name;
    int off;
    vector<int> tp;
};


struct FunctionTable
{
    int level;
    int paraNum;
    int returnType;
    vector<int> para;
};

typedef vector<StructTableVal> StructTableEntry;






class SymbolTable : public Table<SymbolEntry> {
private:
    vector<StructTableEntry> StructTable;

public:
    void fillStructVal(string name);
    void entry(string n, int type, Catgory c, int addr);
    void print();
    friend bool operator<(const SymbolEntry &ls, const SymbolEntry &rs);
};









#endif //COMPILER_SYMBOLTABLE_H
