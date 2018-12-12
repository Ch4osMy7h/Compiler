//
// Created by thankod on 18-12-10.
//
#include "MyUtils.h"
#include <cctype>
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
