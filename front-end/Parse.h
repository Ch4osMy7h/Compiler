//
// Created by chaosmyth on 2018/11/21.
//

#ifndef COMPILER_PARSE_H
#define COMPILER_PARSE_H

#include <bits/stdc++.h>
#include "../QuadTuple.h"
#include "../Token.h"
#include "../SymbolTable.h"
#include "../TypeTable.h"
#include "../MyUtils.h"

using namespace std;



class Parse {
private:
    vector<QuadTuple>& quadVec;
    vector<Token>& tokenVec;
    SymbolTable& st;
    KeyWordTable& keyWordTable;
    IdentiferTable& identiferTable;
    DelimiterTable& delimiterTable;
    IntTable& intTable;
    FloatTable& floatTable;
    CharTable& charTable;
    StringTable& stringTable;
    stack<string> sem;
    int curIndex;
    int curFun;
    int funCnt;
    int curSymInd;
    string curType;
    int paraNum; //记录参数个数
    vector<string> paramType;
    vector<string> paramName;

    //函数调用传入参数
    vector<string> paramCall;


    //用于赋值表达式的生成
    string varName;
    Type varType;
    string expName;
    Type expType;
    string simExpName;
    Type simExpType;

    int t_num = -1;

    //用于算术表达式等的生成
    string relopTmp; Type relopType;
    string addTmp;  Type addType;
    string multiTmp; Type multiType;
    string callTmp; Type callType;




    string relopName;
    string addopName;
    string multiopName;
    string curTmpName;


public:
    Parse(vector<QuadTuple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
              IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
              StringTable &stTable);
    void parse();
    int program();
    int declarationList();             //声明列表
    int declaration();                  //声明
    int varDeclaration();              //变量声明
    int typeSpecifier();               //数据类型定义
    int funDeclaration();              //函数声明
    int params();                      //参数声明
    int paramList();                   //参数列表
    int param();                       //单个参数
    int compoundStmt();                 //复合语句
    int localDeclarations();           //本地变量声明
    int statementList();               //语句列表
    int statement();                    //语句
    int expressionStmt();              //表达式语句
    int selectionStmt();               //选择语句
    int iterationStmt();               //循环语句
    int returnStmt();                  //返回语句
    int expression();                   //表达式
    int var();                          //变量
    int simpleExpression();            //简单表达式
    int relop();                        //简单表达式符号
    int additiveExpression();          //加法表达式
    int addop();                        //加法表达式符号
    int term();                         //乘法表达式项
    int mulop();                        //乘法表达式符号
    int factor();                       //乘法表达式的因子
    int call();                         //函数调用
    int args();                         //调用参数
    int argList();                     //调用参数列表
    bool isType(Token token);           //是否是类型关键字
    bool isNum(Token &token);           //是否是数字
    unsigned int typeSize(string type);


    string toSymTypeName(Type &type);

    Type toType(string basic_string);

    void print();

    bool typePriority(Type left, Type right);

    bool isChar(Token &token);

    Type typeSwitch(Type type, Type addType);

    int varDeclarationList();
};



#endif //COMPILER_PARSE_H
