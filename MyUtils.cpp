//
// Created by thankod on 18-12-10.
//
#include "MyUtils.h"
#include <cctype>
#include "SymbolTable.h"
bool isNum(string str)
{
    return !str.empty() && isdigit(str[0]);
}

string calculateNum(string op, string a, string b)
{
    if(op == "+") {
        return to_string(string2num<int>(a) + string2num<int>(b));
    } else if(op == "-") {
        return to_string(string2num<int>(a) - string2num<int>(b));
    } else if(op == "*") {
        return to_string(string2num<int>(a) * string2num<int>(b));
    } else if(op == "/") {
        return to_string(string2num<int>(a) / string2num<int>(b));
    } else if(op == "<") {
        return to_string(string2num<int>(a) < string2num<int>(b));
    } else if(op == ">") {
        return to_string(string2num<int>(a) > string2num<int>(b));
    } else if(op == "==") {
        return to_string(string2num<int>(a) == string2num<int>(b));
    }
}

bool isOperator(string str)
{
    return str == "+" || str == "-" || str == "*" || str == "/" ||
            str == "<" || str == ">" || str == "==" || str == "=";

}


bool isTempName(string name, string curFun)
{
    return symbolTable.isTempName(name, curFun);
}

bool isActive(string name, string curFun)
{
    return symbolTable.isActive(name, curFun);
}

void setActive(string name, string curFun, bool active)
{
    symbolTable.setActive(name, curFun, active);
}


pair<bool, int> getAddrFromTable(string name, string curFun)
{
    return symbolTable.getAddr(name, curFun);
}

int getFuncSize(string curFun)
{
    return symbolTable.getFuncSize(curFun);
}


bool isGlobalName(string name, string curFun)
{
    return symbolTable.getAddr(name, curFun).first;
}
