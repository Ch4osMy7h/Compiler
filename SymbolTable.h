//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include "Table.h"

using namespace std;

enum Type {
    INT = 1,
    DOUBLE = 2,
    CHAR = 3,
    BOOL = 4,
    ARRAY = 5
};



struct ConstTable {
    double num;
};




struct ArrayInfo {
    unsigned int low, up;
    Type type;
    unsigned int clen;
};

struct ParamInfo {
    string name;
    Type type;
    //vector<ArrayInfo> ai;
    //int vall;
    bool isFormalParam;
    ParamInfo(string n, Type t, bool is) {
        name = n;
        type = t;
        isFormalParam = is;
    }
};

struct FunctionTable {
    int fn;
    vector<ParamInfo> pi;
    int entry;
};


struct Vall {

};


enum Category {
    FUNCTION = 1,
    CONST = 2,
    VARIABLE = 3,
    NONE = 4
};

struct SymbolTableElement {
    string name;
    Type type;
    Category cat;
    int addr;
    ConstTable* ct{};
    int len{};
    int ai;
    Vall* vall{};
    SymbolTableElement() {
        name = "";
        type = INT;
        //cat = -1;
        cat = NONE;
        addr = -1;
        ai = -1;
    }
};





class SymbolTable : public Table<SymbolTableElement> {
public:
    vector<FunctionTable> functionTableVec;
    vector<ArrayInfo> arrayTableVec;
    void fillStructVal(string name);
    void print();
    int searchSymbolName(string name);
    friend bool operator<(const SymbolTableElement& ls, SymbolTableElement& rs);
};









#endif //COMPILER_SYMBOLTABLE_H
