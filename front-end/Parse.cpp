//
// Created by chaosmyth on 2018/11/21.
//

#include "Parse.h"
#include "../MyUtils.h"
#include "../SymbolTable.h"

using namespace std;


/*
 * To_Do_list:
 * 缺少对于关键字的特殊判断语句
 * 变量重定义的作用域判断
 * 缺少对于bool类型的判断
 */




Parse::Parse(vector<QuadTuple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
    this->curFun = 0;
    this->paraNum = 0;
}

int Parse::program() {
    if(declarationList()) {
        return 1;
    }
    return 0;
}


int Parse::declarationList() {
    if(curIndex == tokenVec.size()) {
        return 0;
    }
    int index = curIndex;
    int helper_fun = curFun;
    int helper_func = funCnt;
    if(!declaration()) {
        curIndex = index;
        curIndex = index;
        curFun = helper_fun;
        funCnt = helper_func;
        return 0;
    }
    declarationList();
    return 1;
}

int Parse::declaration() {
    int index = curIndex;
    if(funDeclaration()) return 1;
    curIndex = index;
    if(varDeclaration()) return 1;
    curIndex = index;
    return 0;
}

int Parse::varDeclaration() {
    int index = curIndex;
    if(typeSpecifier()) {
        if(tokenVec[curIndex].type != TokenType::IDENTIFIER) {
            cout << "Error occured when missing variable name or function name" << endl;
            exit(0);
        }
        string curName = tokenVec[curIndex].name;
        //全局变量
        int stIndex = 0;
        stIndex = st.searchSymbolName(tokenVec[curIndex].name, true);
        if(stIndex >= 0) {
            cout << "redefined variable or function name" << endl;
            exit(0);
        }
        curSymInd++;
        st.symbolTable[curFun].emplace_back(SymbolTableElement());
        int symind = static_cast<int>(st.symbolTable[funCnt].size() - 1);
        st.symbolTable[curFun][symind].name = curName;
        st.symbolTable[curFun][symind].cat = Category::VARIABLE;


        bool arrayFlag = false;
        curIndex++;
        int arrLen = 0;

        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type ==  INTCONST) {
                arrLen = string2num<int >(tokenVec[curIndex].name);
                curIndex++;
            } else {
                cout << "error array size" << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
            } else {
                cout << "miss an ] flag when defina an array" << endl;
                exit(0);
            }
            arrayFlag = true;
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "变量声明缺少；语句" << endl;
        }
        if(arrayFlag) {
            quadVec.emplace_back("vardef", curName, "arr", "__");
            ArrayInfo arrayInfoEle{};
            arrayInfoEle.low = 0;
            arrayInfoEle.type = toType(curType);
            arrayInfoEle.up = static_cast<unsigned int>(arrLen - 1);
            arrayInfoEle.clen = typeSize(curType);
            st.arrayTableVec.push_back(arrayInfoEle);
            st.symbolTable[funCnt][symind].type = ARRAY;
            st.symbolTable[funCnt][symind].aiInd = static_cast<int>(st.arrayTableVec.size() - 1);
            st.symbolTable[funCnt][symind].len = arrayInfoEle.clen * arrLen;
        }else {
            quadVec.emplace_back("vardef", curName, curType, "--");
            st.symbolTable[funCnt][symind].type = toType(curType);
            /*
             * vall
             */
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::typeSpecifier() {
    int index = curIndex;
    if(tokenVec[curIndex].type == KEYWORD && isType(tokenVec[curIndex])) {
        curType = tokenVec[curIndex].name;
        curIndex++;
        return 1;
    } else {
        curIndex = index;
        return 0;
    }
}

bool Parse::isType(Token token) {
    return token.type == KEYWORD && (token.id == keyWordTable.index["int"] || token.id == keyWordTable.index["double"] || token.id == keyWordTable.index["void"] || token.id == keyWordTable.index["char"]);
}

int Parse::funDeclaration() {
    if(typeSpecifier()) {
        string curName;
        if(tokenVec[curIndex].type == IDENTIFIER) {
            curName = tokenVec[curIndex].name;
            curIndex++;
        } else {
            cout << "Error occured when missing variable name or function name" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
           return 0;
        }
        paramName.clear();
        paramType.clear();
        params();
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "Error occured when missing \")\" char" << endl;
            exit(0);
        }
        //函数开始四元式生成
        quadVec.emplace_back("fundef", curName, curType, to_string(paraNum));

        vector<SymbolTableElement> stelement;
        stelement.emplace_back( SymbolTableElement());
        FunctionTable ft;
        ft.paramNum = paraNum;
        //ft.off
        st.symbolTable.emplace_back(stelement);
        curFun++;
        st.symbolTable[curFun][0].name = curName;
        st.symbolTable[curFun][0].type = toType(curType);
        st.symbolTable[curFun][0].cat = Category ::FUNCTION;

//        st.tokenTable[curSymInd].name = curName;
//        st.tokenTable[curSymInd].type = toType(curType);
//        st.tokenTable[curSymInd].cat = Category::FUNCTION;
//        st.tokenTable[curSymInd].addr = static_cast<int>(st.functionTableVec.size());
//        FunctionTable ft;
//        ft.fn = paraNum;
        auto iter_type = paramType.begin(); auto iter_name = paramName.begin();
        while(iter_type != paramType.end() && iter_name != paramName.end()) {
            ft.pi.emplace_back(ParamInfo(*iter_name, toType(*iter_type), true));
            quadVec.emplace_back("paradef", *iter_type, *iter_name, "__");
            iter_type++; iter_name++;
        }
        paramType.clear();
        paramName.clear();
        paraNum = 0;
        st.functionTableVec.push_back(ft);
        //符号表链接
        st.symbolTable[curFun][0].pfinalind = static_cast<int>(st.functionTableVec.size() - 1);




        if(compoundStmt()) {
            quadVec.emplace_back("funend", "__", "__", "__");
            return 1;
        } else {
            cout << "函数定义错误" << endl;
        }
    }
    return 0;
}

int Parse::params() {
    int index = curIndex;
    if (paramList()) {
        return 1;
    }
    curIndex = index;
    return 1;
}

int Parse::paramList() {
    int index = curIndex;
    if(!param()) {
        curIndex = index;
        return 0;
    }
    paraNum++;
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id) {
        curIndex++;
        if(!paramList()) {
            cout << "Error occured when need a param after\",\"" << endl;
            exit(0);
        }
    }
    return 1;
}

int Parse::param() {
    int index = curIndex;
    if(typeSpecifier()) {
        bool arrayFlag = false;
        string curName;
        if(tokenVec[curIndex].type == IDENTIFIER) {
            curName = tokenVec[curIndex].name;
            curIndex++;
        } else {
            cout << "Error occured when need an identifer after type" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
                arrayFlag = true;
            } else {
                cout << "Error occured when missing \"]\" char" << endl;
                exit(0);
            }
        }
        if(arrayFlag) {
            paramType.emplace_back("arr");
            paramName.push_back(curName);
            return 1;
        }
        paramType.push_back(curType);
        paramName.push_back(curName);
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::compoundStmt() {
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["{"] == tokenVec[curIndex].id) {
        curIndex++;
        localDeclarations();
        statementList();
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["}"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            //cout << curIndex << endl;
            cout << "Error occured when missing \"}\" char" << endl;
            exit(0);
        }
        return 1;
    }
    return 0;
}

int Parse::statementList() {
    int index = curIndex;
    if (statement()) {
        statementList();
        return 1;
    }
    curIndex = index;
    return 1;
}

int Parse::statement() {
    int index = curIndex;
    if(expressionStmt()) {
        return 1;
    }
    curIndex = index;
    if(compoundStmt(true)) {
        return 1;
    }
    curIndex = index;
    if(selectionStmt()) {
        return 1;
    }
    curIndex = index;
    if(iterationStmt()) {
        return 1;
    }
    curIndex = index;
    if(returnStmt()) {
        return 1;
    }
    return 0;
}

int Parse::expressionStmt() {
    //bool isExpression = false;
    if(expression()) {
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
            return 1;
        } else {
            cout << "Errored when missing ; char" << endl;
            exit(0);
        }
    }

    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
       curIndex++;
       return 1;
    }
    return 0;
}

int Parse::selectionStmt() {
    if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["if"] == tokenVec[curIndex].id) {
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
            quadVec.emplace_back(QuadTuple("if", "__", "__", "__"));
        } else {
            cout << "if语句缺少左括号" << endl;
            exit(0);
        }
        if(!expression()) {
            cout << tokenVec[curIndex].name << endl;
            cout << "if语句表达式错误 " << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << curIndex << endl;
            cout << tokenVec[curIndex].name  << endl;
            cout << "if语句缺少右括号" << endl;
            exit(0);
        }
        quadVec.emplace_back(QuadTuple("ifbegin", curTmpName, "__", "__"));
        //sem.pop();
        if(!statement()) {
            cout << "statement语句表达式错误" << endl;
            exit(0);
        }
        if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["else"] == tokenVec[curIndex].id) {
            quadVec.emplace_back(QuadTuple("else", "__", "__", "__"));
            curIndex++;
            if(!statement()) {
                cout << "else 语句表达式错误" << endl;
                exit(0);
            }
        }
        quadVec.emplace_back(QuadTuple("ifend", "__", "__", "__"));
        return 1;
    }
    return 0;
}

int Parse::iterationStmt() {
    if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["while"] == tokenVec[curIndex].id) {
        curIndex++;
        quadVec.emplace_back("while", "__", "__", "__");
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "while缺少左括号"<< endl;
            exit(0);
        }
        if(!expression()) {
            cout << "while条件语句表达式错误" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
           curIndex++;
            quadVec.emplace_back("do", curTmpName, "__", "__");
            //sem.pop();
        } else {
            cout <<"while缺少右括号" << endl;
            exit(0);
        }
        if(!statement()) {
            cout << "while函数定义出现错误" << endl;
            exit(0);
        }
        quadVec.emplace_back("whileend", "__", "__", "__");
        return 1;
    }
    return 0;
}

int Parse::returnStmt() {
    if(tokenVec[curIndex].type == KEYWORD && keyWordTable.index["return"] == tokenVec[curIndex].id) {
        curIndex++;
        if(!expression()) {
            quadVec.emplace_back("return", "__", "__", "__");
        } else {
            //return语句四元式设计
            quadVec.emplace_back("return", expName, "__", "__");
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout<<"return 语句缺少';'!" << endl;
            exit(0);
        }
        return 1;
    }
    return 0;
}

int Parse::expression() {
    /*
     * 后期加入类型不匹配检查
     */



    int index = curIndex;
    string tmpExp;
    string tmpVar;
    Type tmpExpType;
    Type tmpVarType;


    if(var()) {
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["="] == tokenVec[curIndex].id) {
            curIndex++;
            tmpVar = varName;
            tmpVarType = varType;
            if (expression()) {
                tmpExp = expName;
                tmpExpType = expType;
                if(tmpExpType != tmpVarType) {
                    cout << "类型不匹配" << endl;
                }
            } else {
                cout << "表达式错误" << endl;
                exit(0);
            }
            quadVec.emplace_back("=", tmpExp, "__", tmpVar);
            expName = tmpVar;
            expType = tmpExpType;
            //符号表
            return 1;
        }
    }
    curIndex = index;
    if(simpleExpression()) {
        expName = simExpName;
        expType = simExpType;
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::var() {
    string helper;
    if(tokenVec[curIndex].type == IDENTIFIER) {
        helper = tokenVec[curIndex].name;
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(!expression()) {
                cout << "数组内表达式错误" << endl;
//                cout << curIndex << endl;
//                cout << tokenVec[curIndex-1].name << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                varName = helper + "[" + expName + "]";
                curIndex++;
            } else {
                cout << "数组缺少]" << endl;
                exit(0);
            }
            return 1;
        }
        varName = helper;
        return 1;
    }
    return 0;
}

int Parse::simpleExpression() {
    int index = curIndex;
    string helper;
    if(additiveExpression()) {
        helper = relopTmp;
        if(relop()) {
            if (additiveExpression()) {
                curTmpName = "t" + to_string(++t_num);
                quadVec.emplace_back(relopName, helper, relopTmp, curTmpName);
                expName = curTmpName;
            } else {
                cout << "表达式错误" << endl;
                exit(0);
            }
        }
        simExpName = relopTmp;
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::relop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["<="] == tokenVec[curIndex].id || delimiterTable.index["<"] == tokenVec[curIndex].id || delimiterTable.index[">"] == tokenVec[curIndex].id || delimiterTable.index[">="] == tokenVec[curIndex].id || delimiterTable.index["=="] == tokenVec[curIndex].id || delimiterTable.index["!="] == tokenVec[curIndex].id )) {
        relopName = tokenVec[curIndex].name;
        curIndex++;
        return 1;
    }
    return 0;
}

int Parse::additiveExpression() {
    string helper;
    if(term())  {
        helper = addTmp;
        if(addop()) {
            if(!term()) {
                cout << "addop后面缺少表达式" << endl;
                exit(0);
            }
            curTmpName = "t"+to_string(++t_num);
            quadVec.emplace_back(addopName, helper, addTmp, curTmpName);
            relopTmp = curTmpName;
            return 1;
        }
        relopTmp = addTmp;
        return 1;
    }
    return 0;
}

int Parse::addop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["+"] == tokenVec[curIndex].id ||delimiterTable.index["-"] == tokenVec[curIndex].id )) {
        addopName = tokenVec[curIndex].name;
        curIndex++;
        return 1;
    }
    return 0;
}

int Parse::term() {
    string helper_factor, helper_term;
    if (factor()) {
        helper_factor = multiTmp;
        if (mulop()) {
            if (term()) {
                helper_term = addTmp;
                curTmpName = "t" + to_string(++t_num);
                quadVec.emplace_back(multiopName, helper_factor, helper_term, curTmpName);
                addTmp = curTmpName;
                return 1;
            } else {
                cout << "缺少表达式" << endl;
                exit(0);
            }
        }
        addTmp = multiTmp;
        return 1;
    }
    return 0;
}

int Parse::mulop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["*"] == tokenVec[curIndex].id ||delimiterTable.index["/"] == tokenVec[curIndex].id )) {
        multiopName = tokenVec[curIndex].name;
        curIndex++;
        return 1;
    }
    return 0;
}

int Parse::factor() {
    int index = curIndex;
    if(call()) {
        return 1;
    }
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
        curIndex++;
        if(expression()) {
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
                curIndex++;
                multiTmp = expName;
                return 1;
            } else {
                cout << curIndex << " " << tokenVec[curIndex-1].name << endl;
                cout << "缺少)" << endl;
                exit(0);
            }
        } else {
            cout << "表达数错误" << endl;
            exit(0);
        }
    }
    if(isNum(tokenVec[curIndex])) {
        multiTmp = tokenVec[curIndex].name;
        curIndex++;
        return 1;
    }
    if(var()) {
        multiTmp = varName;
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::call() {
    int index = curIndex;
    string curName;
    if(tokenVec[curIndex].type == IDENTIFIER) {
        curName = tokenVec[curIndex].name;
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            quadVec.emplace_back("funcall", curName, "__", "__");
            curIndex++;
            if(args()) {
                if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
                    curIndex++;
                } else {
                    cout << "函数调用缺少右括号" << endl;
                    exit(0);
                }
            } else {
                cout << "函数调用参数错误" << endl;
                exit(0);
            }
        } else {
            curIndex = index;
            return 0;
        }
        quadVec.emplace_back("callend", "__", "__", "__");
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::args() {
    if(argList()) {
        return 1;
    }
    return 1;
}

int Parse::argList() {
    if(expression())  {
        paramCall.emplace_back(expName);
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id) {
            curIndex++;
            if (!argList()) {
                cout << ",后没有参数定义" << endl;
                exit(0);
            }
        }
        for(auto val : paramCall) {
            quadVec.emplace_back("paramin", val, "__", "__");
        }
        paramCall.clear();
        return 1;
    }
    return 0;
}



int Parse::localDeclarations() {
    if(varDeclaration()) {
        localDeclarations();
        return 1;
    }
    return 1;
}

bool Parse::isNum(Token &token) {
    return token.type == FLOATCONST ||  token.type == INTCONST || token.type == CHARCONST;
}

void Parse::parse() {
    if(!program())  {
        cout << "程序运行错误" << endl;
    }
    //cout << curIndex << endl;
}

unsigned int Parse::typeSize(string curType) {
    if(curType == "int") {
        return 4;
    } else if(curType == "double" || curType == "float") {
        return 8;
    } else if(curType == "char") {
        return 1;
    }
    return 0;
}



string Parse::toSymTypeName(Type &type) {

    if(type == Type::INT) {
        return "int";
    } else if(type == Type::BOOL) {
        return "bool";
    } else if(type == Type::CHAR) {
        return "char";
    } else if(type == Type::DOUBLE) {
        return "double";
    }
    return "";
}

Type Parse::toType(string basic_string) {
    if(basic_string == "int") {
        return Type::INT;
    } else if(basic_string == "bool") {
        return Type::BOOL;
    } else if(basic_string == "float" || basic_string == "double") {
        return Type ::DOUBLE;
    } else if(basic_string == "char") {
        return Type ::CHAR;
    } else if(basic_string == "arr") {
        return Type ::ARRAY;
    }
}

void Parse::print() {
    for(auto &i: quadVec) {
        i.print();
    }
}











