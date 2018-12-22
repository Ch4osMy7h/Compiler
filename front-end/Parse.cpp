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
 * 数组越界
 * 是否赋值的判断
 */




Parse::Parse(vector<QuadTuple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
    this->curFun = 0;
    this->paraNum = 0;
    this->funCnt = 0;
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
            cout << "类型声明后必须跟着标识符在第" << tokenVec[curIndex].line << "行" << endl;
            exit(0);
        }
        string curName = tokenVec[curIndex].name;
        int stIndex = 0;
        stIndex = st.searchSymbolName(tokenVec[curIndex].name, curFun);
        if(stIndex >= 0) {
//            cout << tokenVec[curIndex].name << endl;
            cout << "变量"<<  tokenVec[curIndex].name <<"重定义" << "在第" << tokenVec[curIndex].line  << "行" << endl;
           exit(0);
        }

        curSymInd++;
        st.symbolTable[curFun].emplace_back(SymbolTableElement());
        int symind = static_cast<int>(st.symbolTable[curFun].size() - 1);
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
                cout << "数组长度必须是整型变量在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
            } else {
                cout << "数组声明缺少右括号在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            arrayFlag = true;
        }
        if(arrayFlag) {
            quadVec.emplace_back("vardef", curName, "arr", "__");
            ArrayInfo arrayInfoEle{};
            arrayInfoEle.low = 0;
            arrayInfoEle.type = toType(curType);
            arrayInfoEle.up = static_cast<unsigned int>(arrLen - 1);
            arrayInfoEle.clen = typeSize(curType);
            st.arrayTableVec.push_back(arrayInfoEle);
            st.symbolTable[curFun][symind].type = ARRAY;
            st.symbolTable[curFun][symind].aiInd = static_cast<int>(st.arrayTableVec.size() - 1);
            st.symbolTable[curFun][symind].len = arrayInfoEle.clen * arrLen;
            st.symbolTable[curFun][symind].vall = st.vallVec[curFun];
            st.symbolTable[curFun][symind].vall = st.vallVec[curFun];
            st.vallVec[curFun] += arrayInfoEle.clen * arrLen;
        }else {
            quadVec.emplace_back("vardef", curName, curType, "--");
            st.symbolTable[curFun][symind].type = toType(curType);
            st.symbolTable[curFun][symind].vall = st.vallVec[curFun];
            st.vallVec[curFun] += typeSize(curType);
        }
        varDeclarationList();
        return 1;
    }
    curIndex = index;
    return 0;
}

int Parse::varDeclarationList() {
    int index = curIndex;
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[","] == tokenVec[curIndex].id) {
        curIndex++;
            if(tokenVec[curIndex].type != TokenType::IDENTIFIER) {
                cout << "类型声明后必须跟着标识符在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            string curName = tokenVec[curIndex].name;
            int stIndex = 0;
            stIndex = st.searchSymbolName(tokenVec[curIndex].name, curFun);
            if(stIndex >= 0) {
//            cout << tokenVec[curIndex].name << endl;
                cout << "变量"<<  tokenVec[curIndex].name <<"重定义" << "在第" << tokenVec[curIndex].line  << "行" << endl;
                exit(0);
            }

            curSymInd++;
            st.symbolTable[curFun].emplace_back(SymbolTableElement());
            int symind = static_cast<int>(st.symbolTable[curFun].size() - 1);
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
                    cout << "数组长度必须是整型变量在第" << tokenVec[curIndex].line << "行" << endl;
                    exit(0);
                }
                if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                    curIndex++;
                } else {
                    cout << "数组声明缺少右括号在第" << tokenVec[curIndex].line << "行" << endl;
                    exit(0);
                }
                arrayFlag = true;
            }
            if(arrayFlag) {
                quadVec.emplace_back("vardef", curName, "arr", "__");
                ArrayInfo arrayInfoEle{};
                arrayInfoEle.low = 0;
                arrayInfoEle.type = toType(curType);
                arrayInfoEle.up = static_cast<unsigned int>(arrLen - 1);
                arrayInfoEle.clen = typeSize(curType);
                st.arrayTableVec.push_back(arrayInfoEle);
                st.symbolTable[curFun][symind].type = ARRAY;
                st.symbolTable[curFun][symind].aiInd = static_cast<int>(st.arrayTableVec.size() - 1);
                st.symbolTable[curFun][symind].len = arrayInfoEle.clen * arrLen;
            }else {
                quadVec.emplace_back("vardef", curName, curType, "--");
                st.symbolTable[curFun][symind].type = toType(curType);
                st.symbolTable[curFun][symind].vall = st.vallVec[curFun];
                st.vallVec[curFun] += typeSize(curType);
            }
            varDeclarationList();
    } else if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
        curIndex++;
        return 1;
    } else {
        cout << "变量声明缺少;" << tokenVec[curIndex].line  << "行" << endl;
        exit(0);
    }
    return 1;
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
    return token.type == KEYWORD && (token.id == keyWordTable.index["int"] || token.id == keyWordTable.index["double"] || token.id == keyWordTable.index["void"] || token.id == keyWordTable.index["char"] || token.id == keyWordTable.index["bool"]);
}

int Parse::funDeclaration() {
    if(typeSpecifier()) {
        string curName;
        string tmpType = curType;
        int line = 0;
        if(tokenVec[curIndex].type == IDENTIFIER) {
            curName = tokenVec[curIndex].name;
            line = tokenVec[curIndex].line;
            curIndex++;
        } else {
            cout << "类型声明后必须跟着标识符在第" << tokenVec[curIndex].line << "行" << endl;
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
            cout << "函数定义缺少右括号在第" << tokenVec[curIndex].line << "行" << endl;
            exit(0);
        }
        //函数开始四元式生成
        curType = tmpType;
        quadVec.emplace_back("fundef", curName, curType, to_string(paraNum));
        vector<SymbolTableElement> stelement;
        stelement.emplace_back( SymbolTableElement());
        FunctionTable ft;
        ft.paramNum = paraNum;
        //ft.off
        st.symbolTable.emplace_back(stelement);
        curFun = ++funCnt;
        st.symbolTable[curFun][0].name = curName;
        st.symbolTable[curFun][0].type = toType(curType);
        st.symbolTable[curFun][0].cat = Category ::FUNCTION;
        st.funToName[curName] = curFun; //函数名映射
        st.vallVec.push_back(2);


//        st.tokenTable[curSymInd].name = curName;
//        st.tokenTable[curSymInd].type = toType(curType);
//        st.tokenTable[curSymInd].cat = Category::FUNCTION;
//        st.tokenTable[curSymInd].addr = static_cast<int>(st.functionTableVec.size());
//        FunctionTable ft;
//        ft.fn = paraNum;
        auto iter_type = paramType.begin(); auto iter_name = paramName.begin();
        while(iter_type != paramType.end() && iter_name != paramName.end()) {
            ft.pi.emplace_back(ParamInfo(*iter_name, toType(*iter_type), true));
            st.symbolTable[curFun].emplace_back(SymbolTableElement());
            int symInd = static_cast<int>(st.symbolTable[curFun].size() - 1);
            st.symbolTable[curFun][symInd].name = *iter_name;
            st.symbolTable[curFun][symInd].type = toType(*iter_type);
            st.symbolTable[curFun][symInd].cat = Category ::FORMALPARAM;
            st.symbolTable[curFun][symInd].vall = st.vallVec[curFun];
            st.vallVec[curFun] += typeSize(*iter_type);

//            st.symbolTable[curFun][symInd].vall = 0;
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
            curFun = 0;
            return 1;
        } else {
            cout << "函数" << curName  << "定义错误在第" << line << "行" << endl;
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
            cout << "在" << tokenVec[index].line << "行缺少形式参数声明" << endl;
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
            cout << "类型声明后必须跟着标识符在第" << tokenVec[curIndex].line << "行" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["["] == tokenVec[curIndex].id) {
            curIndex++;
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["]"] == tokenVec[curIndex].id) {
                curIndex++;
                arrayFlag = true;
            } else {
                cout << "数组声明缺少右括号在第"<< tokenVec[curIndex].line << "行" << endl;
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
            //cout << tokenVec[curIndex].name << endl;
            cout << "函数定义缺少右括号在第" << tokenVec[curIndex].line- 1  << "行" << endl;
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
    if(compoundStmt()) {
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
            cout << "表达式语句缺少;在第" << tokenVec[curIndex].line << "行"<< endl;
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
        int line = tokenVec[curIndex].line;
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
            curIndex++;
            quadVec.emplace_back(QuadTuple("if", "__", "__", "__"));
        } else {
            cout << "if语句缺少左括号在第" << tokenVec[curIndex].line  << "行" << endl;
            exit(0);
        }
        if(!expression()) {
            cout << tokenVec[curIndex].name << endl;
            cout << "if语句表达式错误在第" << tokenVec[curIndex].line  << "行" << endl;
            exit(0);
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout << curIndex << endl;
            cout << tokenVec[curIndex].name  << endl;
            cout << "if语句缺少右括号在第" << tokenVec[curIndex].line  << "行" << endl;
            exit(0);
        }
        quadVec.emplace_back(QuadTuple("ifbegin", curTmpName, "__", "__"));
        //sem.pop();
        if(!statement()) {
            cout << "statement语句表达式错误在第" << tokenVec[curIndex].line  << "行" << endl;
            exit(0);
        }
        if (tokenVec[curIndex].type == KEYWORD && keyWordTable.index["else"] == tokenVec[curIndex].id) {
            quadVec.emplace_back(QuadTuple("else", "__", "__", "__"));
            curIndex++;
            int elseLine = tokenVec[curIndex].line;
            if(!statement()) {
                cout << "else 语句表达式错误在第"  << elseLine << "行" << endl;
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
            cout << "while缺少左括号在第" << tokenVec[curIndex].line << "行" << endl;
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
            cout <<"while缺少右括号在第" << tokenVec[curIndex].line << "行"<< endl;
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
            Type returnType = st.symbolTable[curFun][0].type;
            //cout << returnType << endl;
            if(returnType != Type::VOID) {
                cout << "函数类型需要返回值在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            quadVec.emplace_back("return", "__", "__", "__");
        } else {
            Type returnType = st.symbolTable[curFun][0].type;
//            cout << returnType << endl;
            if(returnType == Type::VOID) {
                cout << "函数类型是void 不需要返回值在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            //return语句四元式设计
            quadVec.emplace_back("return", expName, "__", "__");
        }
        if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[";"] == tokenVec[curIndex].id) {
            curIndex++;
        } else {
            cout<<"return 语句缺少';'在第" << tokenVec[curIndex].line -1 << "行" << endl;
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
            tmpVarType = st.searchSymbolType(varName, curFun);
            if (expression()) {
                tmpExp = expName;
                tmpExpType = expType;
                //cout << expName << endl;
                if(!typePriority(tmpVarType, tmpExpType)) {
                    cout << toSymTypeName(tmpVarType) <<"类型不匹配" <<toSymTypeName(tmpExpType) <<  endl;
//                    cout << toSymTypeName(tmpExpType) << " " << toSymTypeName(tmpVarType) << endl;
                    exit(0);
                }
            } else {
                cout << "表达式错误在第" << tokenVec[curIndex].line << "行" << endl;
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
                cout << "数组缺少]在第" << tokenVec[curIndex].line << "行" << endl;
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
    Type helper_Type;
    if(additiveExpression()) {
        helper = relopTmp;
        helper_Type = relopType;
        if(relop()) {
            if (additiveExpression()) {
                if(!typePriority(relopType, helper_Type)) {
//                    cout << helper_Type << " " << relopType << endl;
                    cout << toSymTypeName(helper_Type) <<"类型不匹配" << toSymTypeName(relopType)<< endl;
                    exit(0);
                }
                curTmpName = "t" + to_string(++t_num);
                quadVec.emplace_back(relopName, helper, relopTmp, curTmpName);
                st.symbolTable[curFun].push_back(SymbolTableElement());
                auto symInd = st.symbolTable[curFun].size() - 1;
                st.symbolTable[curFun][symInd].name = curTmpName;
                st.symbolTable[curFun][symInd].type = Type::BOOL;
                st.symbolTable[curFun][symInd].cat = Category ::CONST;
                st.symbolTable[curFun][symInd].isTemp = true;
                st.symbolTable[curFun][symInd].isActive = false;
                st.symbolTable[curFun][symInd].vall = st.vallVec[curFun];
                st.vallVec[curFun] += 1;
                expName = curTmpName;
            } else {
                cout << "表达式错误" << endl;
                exit(0);
            }
        }
        simExpName = relopTmp;
        simExpType = relopType;
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
    Type helper_Type;
    if(term())  {
        helper = addTmp;
        helper_Type = addType;
        bool inWhile = false;
        while (addop()) {
            if(!term()) {
                cout << "addop后面缺少表达式在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
            if(!typePriority(helper_Type, addType) ) {
                cout << toSymTypeName(helper_Type)<<"类型不匹配"<< toSymTypeName(addType) << endl;
//                cout << helper << " " << addTmp << endl;
//                cout << helper_Type << " " << addType << endl;
                exit(0);
            }

            Type resType = typeSwitch(helper_Type, addType);
            curTmpName = "t"+to_string(++t_num);
            quadVec.emplace_back(addopName, helper, addTmp, curTmpName);
            relopTmp = curTmpName;
            relopType = addType;

            //符号表
            st.symbolTable[curFun].push_back(SymbolTableElement());
            auto symInd = st.symbolTable[curFun].size() - 1;
            st.symbolTable[curFun][symInd].name = curTmpName;
            st.symbolTable[curFun][symInd].type = resType;
            st.symbolTable[curFun][symInd].cat = Category::VARIABLE;
            st.symbolTable[curFun][symInd].isTemp = true;
            st.symbolTable[curFun][symInd].isActive = false;
            st.symbolTable[curFun][symInd].vall = st.vallVec[curFun];
            st.vallVec[curFun] += typeSize(toSymTypeName(resType));

            helper = curTmpName;
            helper_Type = addType;
            inWhile = true;
        }
        if(inWhile) {
            relopTmp = curTmpName;
            relopType = addType;
        } else {
            relopTmp = addTmp;
            relopType = addType;
        }
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
    Type helper_factor_type, helper_term_type;
    string helper_mulop;
    if (factor()) {
        helper_factor = multiTmp;
        helper_factor_type = multiType;
        if (mulop()) {
            helper_mulop = multiopName;
            if (term()) {
                helper_term = addTmp;
                helper_term_type = addType;
                if(!typePriority(helper_factor_type, helper_term_type) ) {
                    cout << "类型不匹配" << endl;
                    exit(0);
                }

                curTmpName = "t" + to_string(++t_num);
                quadVec.emplace_back(helper_mulop, helper_factor, helper_term, curTmpName);
                addTmp = curTmpName;
                addType = helper_factor_type;


                //符号表
                Type resType = typeSwitch(helper_factor_type, helper_term_type);
                st.symbolTable[curFun].push_back(SymbolTableElement());
                auto symInd = st.symbolTable[curFun].size() - 1;
                st.symbolTable[curFun][symInd].name = curTmpName;
                st.symbolTable[curFun][symInd].type = resType;
                st.symbolTable[curFun][symInd].cat = Category ::CONST;
                st.symbolTable[curFun][symInd].isTemp = true;
                st.symbolTable[curFun][symInd].isActive = false;
                st.symbolTable[curFun][symInd].vall = st.vallVec[curFun];
                st.vallVec[curFun] += typeSize(toSymTypeName(resType));
                return 1;
            } else {
                cout << "mulop缺少表达式在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
        }
        addTmp = multiTmp;
        addType = multiType;
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
        /*
         * 验证正确性
         */
        multiTmp = callTmp;
        multiType = callType;
        return 1;
    }
    if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index["("] == tokenVec[curIndex].id) {
        curIndex++;
        if(expression()) {
            if(tokenVec[curIndex].type == DELIMTER && delimiterTable.index[")"] == tokenVec[curIndex].id) {
                curIndex++;
                multiTmp = expName;
                multiType = expType;
                return 1;
            } else {
                //cout << curIndex << " " << tokenVec[curIndex-1].name << endl;
                cout << "缺少)" << endl;
                exit(0);
            }
        } else {
            cout << "factor表达数错误在第" << tokenVec[curIndex].line << "行" << endl;
            exit(0);
        }
    }
    if(isNum(tokenVec[curIndex])) {
        multiTmp = tokenVec[curIndex].name;
        if(tokenVec[curIndex].name.find('.') == string::npos) {
            multiType = Type::INT;
        } else {
            multiType = Type::DOUBLE;
        }
        curIndex++;
        return 1;
    }
    if(isChar(tokenVec[curIndex])) {
        multiTmp = tokenVec[curIndex].name;
        //cout << multiTmp << endl;
        multiType = Type::CHAR;
        curIndex++;
        return 1;
    }


    if(var()) {
        multiTmp = varName;
        multiType = st.searchSymbolType(varName, curFun);
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
                    cout << "函数调用缺少右括号在第" << tokenVec[curIndex].line << "行" << endl;
                    exit(0);
                }
            } else {
                cout << "函数调用参数错误在第" << tokenVec[curIndex].line << "行" << endl;
                exit(0);
            }
        } else {
            curIndex = index;
            return 0;
        }
        curTmpName = "t"+to_string(++t_num);
        quadVec.emplace_back("callend", "__", "__", curTmpName);
        st.symbolTable[curFun].push_back(SymbolTableElement());
        callTmp = curTmpName;

        auto symInd = st.symbolTable[curFun].size() - 1;
        st.symbolTable[curFun][symInd].name = curTmpName;
        st.symbolTable[curFun][symInd].isTemp = true;
        int funIndex = st.funToName[curName];
        Type retunType = st.symbolTable[funIndex][0].type;
        st.symbolTable[curFun][symInd].type = retunType;
        st.symbolTable[curFun][symInd].vall = st.vallVec[curFun];
        st.vallVec[curFun] += toType(toSymTypeName(retunType));



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
                cout << ",后没有参数定义在第" << tokenVec[curIndex].line << "行" << endl;
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
    return token.type == FLOATCONST ||  token.type == INTCONST;
}

void Parse::parse() {
    if(!program())  {
        cout << "程序运行错误" << endl;
    }
    //cout << curIndex << endl;
}

unsigned int Parse::typeSize(string curType) {
    if(curType == "int") {
        return 1;
    } else if(curType == "double" || curType == "float") {
        return 2;
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
    } else if(type == Type::ARRAY) {
        return "arr";
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
    } else if(basic_string == "string") {
        return Type::STRING;
    } else if(basic_string == "void") {
        return Type::VOID;
    }
}

void Parse::print(string filename) {
    ofstream out;
    out.open(filename, ios::out | ios::trunc);
    if(!out) {
        cout << "Open QuadVec File Error" << endl;
    }
    out << quadVec.size() << endl;
    for(auto &i: quadVec) {
        out << setw(8) << i.op << setw(8) << i.name1 << setw(8) << i.name2 << setw(8) << i.res << endl;
    }
    out.close();
}

bool Parse::typePriority(Type left, Type right) {
    return !((left == DOUBLE || left == INT || left == BOOL) && (right == CHAR));
}

bool Parse::isChar(Token &token) {
    return token.type == CHARCONST;
}

Type Parse::typeSwitch(Type left, Type right) {
    if(left == Type::DOUBLE || right == Type::DOUBLE) {
        return Type::DOUBLE;
    }
    if(left == Type::INT || right == Type::INT) {
        return Type::INT;
    }
}












