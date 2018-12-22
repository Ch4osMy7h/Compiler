//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include "Table.h"
#include "MyUtils.h"

using namespace std;

enum Type {
    INT = 1,
    DOUBLE = 2,
    CHAR = 3,
    BOOL = 4,
    ARRAY = 5,
    STRING = 6,
    VOID = 7
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
    //int num;
    ParamInfo(string n, Type t, bool is) {
        name = n;
        type = t;
        isFormalParam = is;
    }
};

struct FunctionTable {
    int paramNum;
    vector<ParamInfo> pi;
    int entry;
    int off;
};


struct Vall {

};


enum Category {
    FUNCTION = 1,
    CONST = 2,
    VARIABLE = 3,
    NONE = 4,
    FORMALPARAM = 5
};

struct SymbolTableElement {
    string name;
    Type type;
    Category cat;  //种类
    int ctInd;  //常数表index
    int len;    //长度表
    int aiInd;  //数组表index
    int vall;   //值单元分配
    int pfinalind; //如果是函数就给他分配函数表
    double constNum; //if这是个const
    bool isTemp; //临时变量
    bool isActive = true; //是否活跃,默认活跃；
};





class SymbolTable {
public:
    int curSymInd = 0;
    //全局变量就用0对应的symbolTable
    vector<vector<SymbolTableElement>> symbolTable; //符号表总表
    vector<FunctionTable> functionTableVec;
    vector<ArrayInfo> arrayTableVec;
    map<string, int> funToName; //函数名映射 到 符号表



    int searchSymbolName(string name, int curInd);
    Type searchSymbolType(string name,int curFun);
    friend bool operator<(const SymbolTableElement& ls, SymbolTableElement& rs);
    void print();
    bool isTempName(string name, string curFun); //判断是否是临时变量
    bool isActive(string name,string curFun); //判断是否活跃
    void setActive(string name, string curFun, bool active); //设置活跃信息
    pair<bool, int> getAddr(string name, string curFun); //当前作用域搜索函数



    SymbolTable() {
        //全局符号表初始化

        vector<SymbolTableElement> allScopeVar;
        symbolTable.push_back(allScopeVar);
        funToName["allScopeVar"] = 0;
    }

};


extern SymbolTable symbolTable;









#endif //COMPILER_SYMBOLTABLE_H
