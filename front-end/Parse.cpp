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
    this->curFun = -1;
    this->funCnt = 0;
    this->curSymInd = -1;
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
        TokenType curType = tokenVec[curIndex].type;
        //全局变量
        if(curFun == -1) {
            int stIndex = 0;
            stIndex = st.searchSymbolName(tokenVec[curIndex].name);
            if(stIndex >= 0) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }
            st.tokenTable.emplace_back(SymbolTableElement());
            curSymInd++;
            st.tokenTable[curSymInd].name = curName;
            st.tokenTable[curSymInd].cat = Category::VARIABLE;
        } else {
            int stIndex = 0;
            stIndex = st.searchSymbolName(tokenVec[curIndex].name);
            if(stIndex >= 0) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }
            curSymInd++;
            st.tokenTable[curSymInd].name = curName;
            st.tokenTable[curSymInd].cat = VARIABLE;
            //st.ft[curFun].pi
        }
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
        //语义动作
        if(curFun == -1) {
            if(arrayFlag) {
                quadVec.emplace_back("vardef", curName, "arr", "__");
                ArrayInfo arrayInfoEle{};
                arrayInfoEle.low = 0;
                arrayInfoEle.type = curType;
                arrayInfoEle.up = static_cast<unsigned int>(arrLen - 1);
                arrayInfoEle.clen = typeSize(curType);
                st.arrayTableVec.push_back(arrayInfoEle);
                st.tokenTable.emplace_back( SymbolTableElement());
                st.tokenTable[curSymInd].type = ARRAY;
                st.tokenTable[curSymInd].ai = static_cast<int>(st.arrayTableVec.size() - 1);
                st.tokenTable[curSymInd].len = arrayInfoEle.clen * arrLen;
            } else {
                quadVec.emplace_back("vardef", curName, toTypeName(curType), "__");
            }
        } else {
            if(arrayFlag) {
                quadVec.emplace_back("vardef", curName, "arr", "__");
            }else {
                quadVec.emplace_back("vardef", curName, "arr", "--");
            }
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::typeSpecifier() {
    int index = curIndex;
    if(tokenVec[curIndex].type == KEYWORD && isType(tokenVec[curIndex])) {
        curIndex++;
        curType = tokenVec[curIndex].type;
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
        quadVec.emplace_back("fundef", curName, toTypeName(TokenType (curType)), paraNum);
        curSymInd++;
        st.tokenTable.emplace_back( SymbolTableElement());
        st.tokenTable[curSymInd].name = curName;
        st.tokenTable[curSymInd].type = Type(curType);
        st.tokenTable[curSymInd].cat = Category::FUNCTION;
        st.tokenTable[curSymInd].addr = static_cast<int>(st.functionTableVec.size());
        FunctionTable ft;
        ft.fn = paraNum;
        auto iter_type = paramType.begin(); auto iter_name = paramName.begin();
        while(iter_type != paramType.end() && iter_name != paramName.end()) {
            ft.pi.emplace_back(ParamInfo(*iter_name, *iter_type, true));
            iter_type++; iter_name++;
            quadVec.emplace_back("paradef", toSymTypeName(*iter_type), *iter_name, "__");
        }
        st.functionTableVec.push_back(ft);

        if(compoundStmt(false)) {
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
            } else {
                cout << "Error occured when missing \"]\" char" << endl;
                exit(0);
            }
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::compoundStmt(bool isOutput) {
    int index = curIndex;
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["{"] == tokenVec[curIndex].id) {
        curIndex++;
        localDeclarations();
        statementList();
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["}"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << curIndex << endl;
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
    bool isExpression = false;
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
    int index = curIndex;
    if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["if"] == tokenVec[curIndex].id) {
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
            quadVec.emplace_back(QuadTuple("if", "##", "##", "##"));
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
        quadVec.emplace_back(QuadTuple("ifbegin", "##", "##", "##"));
        if(!statement()) {
            cout << "statement语句表达式错误" << endl;
            exit(0);
        }
        if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["else"] == tokenVec[curIndex].id) {
            quadVec.emplace_back(QuadTuple("else", "##", "##", "##"));
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
        quadVec.emplace_back("while", "##", "##", "##");
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
            quadVec.emplace_back("whilebegin", "##", "##", "##");
        } else {
            cout <<"while缺少右括号" << endl;
            exit(0);
        }
        if(!statement()) {
            cout << "while函数定义出现错误" << endl;
            exit(0);
        }
        quadVec.emplace_back("whileend", "##", "##", "##");
        return 1;
    }
    return 0;
}

int Parse::returnStmt() {
    if(tokenVec[curIndex].type == KEYWORD && keyWordTable.index["return"] == tokenVec[curIndex].id) {
        curIndex++;
        if(!expression()) {
            quadVec.emplace_back("return", "##", "##", "##");
        } else {
            //quadVec.emplace_back("return", to_string(), "##", "##");
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
    int index = curIndex;
    if(var()) {
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["="] == tokenVec[curIndex].id) {
            curIndex++;
            if (expression()) {

            } else {
                cout << "表达式错误" << endl;
                exit(0);
            }
            //符号表
            return 1;
        }
    }
    curIndex = index;
    if(simpleExpression()) {
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::var() {
    if(tokenVec[curIndex].type == IDENTIFIER) {
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
                curIndex++;
            } else {
                cout << "数组缺少]" << endl;
                exit(0);
            }
            return 1;
        }
        return 1;
    }
    return 0;
}

int Parse::simpleExpression() {
    int index = curIndex;
    if(additiveExpression()) {
        if(relop()) {
            if(additiveExpression()) {

            } else {
                cout << "表达式错误" << endl;
                exit(0);
            }
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::relop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["<="] == tokenVec[curIndex].id || delimiterTable.index["<"] == tokenVec[curIndex].id || delimiterTable.index[">"] == tokenVec[curIndex].id || delimiterTable.index[">="] == tokenVec[curIndex].id || delimiterTable.index["=="] == tokenVec[curIndex].id || delimiterTable.index["!="] == tokenVec[curIndex].id )) {
        curIndex++;
        return 1;
    }
    return 0;
}

int Parse::additiveExpression() {
    if(term())  {
        if(addop()) {
            if(!term()) {
                cout << "addop后面缺少表达式" << endl;
                cout << tokenVec[curIndex].name << endl;
                exit(0);
            }
            return 1;
        }
        return 1;
    }
    return 0;
}

int Parse::addop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["+"] == tokenVec[curIndex].id ||delimiterTable.index["-"] == tokenVec[curIndex].id )) {
        curIndex++;
        return 1;
    }
    return 0;
}

int Parse::term() {
    if(factor()) {
        if(mulop()) {
            if(term()) {
                return 1;
            } else {
                cout << "缺少表达式" << endl;
                exit(0);
            }
        }
        return 1;
    }
    return 0;
}

int Parse::mulop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["*"] == tokenVec[curIndex].id ||delimiterTable.index["/"] == tokenVec[curIndex].id )) {
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
        curIndex++;
        return 1;
    }
    if(var()) {
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::call() {
    int index = curIndex;
    if(tokenVec[curIndex].type == IDENTIFIER) {
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
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
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id)  {
            curIndex++;
            if(!argList())  {
                cout << ",后没有参数定义" << endl;
                exit(0);
            }
        }
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
    return token.type == FLOATCONST ||  token.type == INTCONST;
}

void Parse::parse() {
    if(!program())  {
        cout << "程序运行错误" << endl;
    }
    //cout << curIndex << endl;
}

unsigned int Parse::typeSize(TokenType type) {
    if(curType == INTCONST) {
        return 4;
    } else if(curType == FLOATCONST) {
        return 8;
    } else if(curType == CHARCONST) {
        return 1;
    }
    return 0;
}

string Parse::toTypeName(TokenType type) {
    if(type == TokenType::INTCONST) {
        return "i";
    } else if(type == TokenType::FLOATCONST) {
        return "f";
    } else if(type == TokenType::CHARCONST) {
        return "c";
    }
    cout << "不是合法的数据类型" << endl;
    exit(0);
}

string Parse::toSymTypeName(Type &type) {
    if(type == Type::INT) {
        return "i";
    } else if(type == Type::BOOL) {
        return "b";
    } else if(type == Type::CHAR) {
        return "c";
    } else if(type == Type::DOUBLE) {
        return "f";
    }
    return "";
}











