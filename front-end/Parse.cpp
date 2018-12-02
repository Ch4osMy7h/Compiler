//
// Created by chaosmyth on 2018/11/21.
//

#include "Parse.h"
using namespace std;

Parse::Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
    this->curFun = -1;
    this->funCnt = 0;
}

int Parse::program() {
    int index = curIndex;
    //fun语义动作
    QuadTurple qt("fun", "-1", "*", "*");
    quadVec.push_back(qt);
    if(declarationList()) {
        QuadTurple qt("funend", "-1", "*", "*");
        quadVec.push_back(qt);
        return 1;
    }
    curIndex = index;
    return 0;
}


int Parse::declarationList() {
    if(curIndex == tokenVec.size()) {
        return 0;
    }
    int index = curIndex;
    int fun = curFun;
    int funC = funCnt;
    if(!declaration()) {
        curIndex = index;
        curIndex = index;
        curFun = fun;
        funCnt = funC;
        return 0;
    }
    declaration();
    return 1;
}

int Parse::declaration() {
    //是否有必要回溯
    if(funDeclaration()) return 1;
    if(varDeclaration()) return 1;
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
        if(curFun == -1) {
            int stIndex = 0;
            if(stIndex = st.searchSymbolName(tokenVec[curIndex].name)) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }
        } else {
            int stIndex = 0;
            //符号表怎么判断作用域 ???
            if(stIndex = st.searchSymbolName(tokenVec[curIndex].name)) {
                cout << "redefined variable or function name" << endl;
                exit(0);
            }

        }
        bool arrayFlag = false;
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type ==  INTCONST) {
                curIndex++;
            } else {
                cout << "error array size" << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
                arrayFlag = true;
            } else {
                cout << "miss an ] flag when defina an array" << endl;
                exit(0);
            }
        } else if(tokenVec[curIndex].type == IDENTIFIER) {
            curIndex++;
        } else {
            cout << "error when define variable" << endl;
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "miss an ; flag when define an array" << endl;
        }

        //语义动作
        if(arrayFlag) {
            quadVec.emplace_back(curName, "arr", "0", "##");
        } else {
            quadVec.emplace_back(curName, to_string(curType), "0", "##");
        }
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::typeSpecifier() {
    if(tokenVec[curIndex].type == KEYWORD && isType(tokenVec[curIndex])) {
        curIndex++;
        return 1;
    } else {
        cout << "Uncorreted type specifier" << endl;
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
            curIndex++;
            curName = tokenVec[curIndex].name;
        } else {
            cout << "Error occured when missing variable name or function name" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "Error occured when missing \"(\" char" << endl;
            exit(0);
        }
        params();
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "Error occured when missing \")\" char" << endl;
            exit(0);
        }
        if(compoundStmt(false)) {
            //语义动作 函数四元式
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
        index = curIndex;
        if(!localDeclarations()) curIndex = index;
        index = curIndex;
        if(!statementList()) curIndex = index;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["}"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
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
    expression();
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id) {
       curIndex++;
       return 1;
    } else {
        cout << "expression need an , flag" << endl;
        exit(0);
    }
    //
    return 0;
}

int Parse::selectionStmt() {
    int index = curIndex;
    if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["if"] == tokenVec[curIndex].id) {
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
            quadVec.emplace_back(QuadTurple("if", "##", "##", "##"));
        } else {
            cout << "if语句缺少左括号" << endl;
            exit(0);
        }
        if(!expression()) {
            cout << "if语句表达式错误 " << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << "if语句缺少右括号" << endl;
            exit(0);
        }
        quadVec.emplace_back(QuadTurple("ifbegin", "##", "##", "##"));
        if(!statement()) {
            cout << "statement语句表达式错误" << endl;
            exit(0);
        }
        if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["else"] == tokenVec[curIndex].id) {
            quadVec.emplace_back(QuadTurple("else", "##", "##", "##"));
            curIndex++;
            if(!statement()) {
                cout << "else 语句表达式错误" << endl;
                exit(0);
            }
        }
        quadVec.emplace_back(QuadTurple("ifend", "##", "##", "##"));
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
    if(tokenVec[curIndex].type == KEYWORD && keyWordTable.index["return"] = tokenVec[curIndex].id) {
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
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["]) {
            curIndex++;
            if(!expression()) {
                cout << "数组内表达式错误" << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"]) {
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
    }
    return 0;
}

int Parse::mulop() {
    if(tokenVec[curIndex].type == DELIMTER && (delimiterTable.index["*"] == tokenVec[curIndex].id ||delimiterTable.index["/"] == tokenVec[curIndex].id )) {
        curIndex++;
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
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[" )"]) {
                curIndex++;
            } else {
                cout << "缺少)" << endl;
            }
        } else {
            cout << "表达数错误" << endl;
            exit(0);
        }
    }
    if(isNum(tokenVec[curIndex])) {
        return 1;
    }
    if(var()) {
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::call() {
    if(tokenVec[curIndex].type == IDENTIFIER) {
        curIndex++;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
            if(args()) {
                if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
                    curIndex++;
                } else {
                    cout << "函数调用缺少左括号" << endl;
                }
            } else {
                cout << "函数调用参数错误" << endl;
            }
        }
        return 1;
    }
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

int Parse::solve() {
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











