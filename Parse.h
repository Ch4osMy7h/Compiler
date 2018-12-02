//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_PARSE_H
#define COMPILER_PARSE_H

#include "bits/stdc++.h"
#include "QuadTurple.h"
#include "Token.h"
#include "SymbolTable.h"
#include "TypeTable.h"
#include "util.h"

using namespace std;



class Parse {
private:
    vector<QuadTurple>& quadVec;
    vector<Token>& tokenVec;
    SymbolTable& st;
    KeyWordTable& keyWordTable;
    IdentiferTable& identiferTable;
    DelimiterTable& delimiterTable;
    IntTable& intTable;
    FloatTable& floatTable;
    CharTable& charTable;
    StringTable& stringTable;
    stack<Token> sem;
    int curIndex;
    int curFun;
    int funCnt;
public:
    Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
              IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
              StringTable &stTable);
    int program();                      //程序主入口
    int declarationList();             //声明列表
    int declaration();                  //声明
    int varDeclaration();              //静态变量声明
    int typeSpecifier();               //数据类型定义
    int funDeclaration();              //函数声明
    int params();                       //参数声明
    int paramList();                   //参数列表
    int param();                        //单个参数
    int compoundStmt(bool isOutput);  //复合语句
    int local_declarations();           //本地变量声明
    int statement_list();               //语句列表
    int statement();                    //语句
    int expression_stmt();              //表达式语句
    int selection_stmt();               //选择语句
    int iteration_stmt();               //循环语句
    int return_stmt();                  //返回语句
    int expression();                   //表达式
    int var();                          //变量
    int simple_expression();            //简单表达式
    int relop();                        //简单表达式符号
    int additive_expression();          //加法表达式
    int addop();                        //加法表达式符号
    int term();                         //乘法表达式项
    int mulop();                        //乘法表达式符号
    int factor();                       //乘法表达式的因子
    int call();                         //函数调用
    int args();                         //调用参数
    int arg_list();                     //调用参数列表
    int solve();                       //语法分析
    bool isType(Token token);           //是否是类型关键字
};



#endif //COMPILER_PARSE_H
