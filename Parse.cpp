//
// Created by chaosmyth on 2018/11/21.
//

#include "Parse.h"
using namespace std;






Parse::Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
             IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
             StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
    this->curIndex = 0;
}

void Parse::START() {
    curIndex++;
    S();
    if(curIndex >=tokenVec.size()) {
        cout << "程序没有正常结束" << endl;
        return ;
    }
    cout << "程序运行成功" << endl;
}

void Parse::S() {
    Function();
    Struct();
    Function();
    Mainfunc();
}


void Parse::Struct() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["struct"]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
            //sturct = 1 mark
            curIndex++;
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["{"]) {
                curIndex++;
                Declaration();
                if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["}"]) {
                    Struct();
                } else {
                    cout << "Missing right bracket" << endl;
                    exit(0);
                }
            }
        } else {
            cout << "the Sturct Define need an identifer" << endl;
            exit(0);
        }
    }
}





void Parse::Declaration() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"])) {
        Variable_Declaration();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
            Declaration();
        } else {
            cout << "missing end sign in the out" << endl;
            exit(0);
        }
    }
}

void Parse::Variable_Declaration() {
    DataType();
    Identifer();
    Array();
}

void Parse::DataType() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"])) {
        curIndex++;
    } else {
        cout << "None Legel DataType" << endl;
        exit(0);
    }
}

void Parse::Identifer() {
    if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        curIndex++;
        Array();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[","]) {
            curIndex++;
            identiferFollow();
        }
    }
}

void Parse::Array() {
    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["["]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::INTCONST && intTable.contains(tokenVec[curIndex].id) ){
            curIndex++;
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["]"]) {
                curIndex++;
                Array();
            } else {
                cout << "missing array end ]" << endl;
                exit(0);
            }
        } else {
            cout << "Need a int num to stand for the size of array" << endl;
            exit(0);
        }
    }
}

void Parse::identiferFollow() {
    if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        curIndex++;
        Array();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[","]) {
            curIndex++;
            identiferFollow();
        }
    }
}

void Parse::Function() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["function"]) {
        curIndex++;
        FunctionSon();
    }
}

void Parse::FunctionSon() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"])) {
        curIndex++;
        DataType();
        FunctionLast();
    }  else if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["void"]){
        curIndex++;
        FunctionLast();
    } else {
        cout << "error define function" << endl;
        exit(0);
    }
}

void Parse::FunctionLast() {
    if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
            curIndex++;
            Declaration();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                curIndex++;
                FunctionWithType();
            }
        }
    }
}

void Parse::FunctionWithType() {
    CompoundStatement();
    Return();
}

void Parse::CompoundStatement() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"] || tokenVec[curIndex].id == keyWordTable.index["use"]) ) {
        DeclareAssignment();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
            CompoundStatement();
        } else {
            cout << "error when missing ;" << endl;
            exit(0);
        }
    } else if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        DeclareAssignment();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
            CompoundStatement();
        }
    } else if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["if"]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
            Logic();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                curIndex++;
                // 语义动作 if
                if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["{"]) {
                    curIndex++;
                    CompoundStatement();
                    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["}"]) {
                        curIndex++;
                        Else();
                        // if end 语义动作
                        CompoundStatement();
                    } else {
                        cout <<"error when missing }" << endl;
                        exit(0);
                    }
                } else {
                    cout << "error when missing {" << endl;
                    exit(0);
                }
            } else {
                cout << "error when missing }" << endl;
                exit(0);
            }
        }
    } else if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["while"]) {
        curIndex++;
        // while 语义动作
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
            Logic();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                curIndex++;
                // 语义动作 do
                if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["{"]) {
                    curIndex++;
                    CompoundStatement();
                    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["}"]) {
                        curIndex++;
                        // 循环语句尾部语义动作
                        CompoundStatement();
                    } else {
                        cout <<"error when missing }" << endl;
                        exit(0);
                    }
                } else {
                    cout << "error when missing {" << endl;
                    exit(0);
                }
            } else {
                cout << "error when missing }" << endl;
                exit(0);
            }
        }
    } else if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["for"]) {
        curIndex++;
        // for 语义动作
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
            curIndex++;
            DeclareAssignment();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
                curIndex++;
                Logic();
                if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
                    curIndex++;
                    // do语义动作
                    SuffixExpression();
                    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                        curIndex++;
                        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["{"]) {
                            curIndex++;
                            CompoundStatement();
                            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["}"]) {
                                //尾部语义动作
                                curIndex++;
                                CompoundStatement();
                            } else {
                                cout << "error when missing }" << endl;
                                exit(0);
                            }
                        } else {
                            cout << "error when missing }" << endl;
                            exit(0);
                        }
                    } else {
                        cout << "error when missing )" << endl;
                        exit(0);
                    }
                } else {
                    cout << "error when missing ;" << endl;
                    exit(0);
                }
            } else {
                cout << "error when missing ;" << endl;
                exit(0);
            }
        } else {
            cout << "error when missing for" << endl;
            exit(0);
        }
    }
}


void Parse::Return() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["return"]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::IDENTIFIER || tokenVec[curIndex].type == TokenType::INTCONST || tokenVec[curIndex].type == TokenType::FLOATCONST || tokenVec[curIndex].type == TokenType::STRCONST || tokenVec[curIndex].type == TokenType::CHARCONST ) {
            curIndex++;
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[";"]) {
                curIndex++;
            } else {
                cout << "error when missing ;" << endl;
                exit(0);
            }
        } else {
            cout << "return type is error" << endl;
            exit(0);
        }
    }
}

void Parse::DeclareAssignment() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && (tokenVec[curIndex].id == keyWordTable.index["int"] || tokenVec[curIndex].id == keyWordTable.index["double"] || tokenVec[curIndex].id == keyWordTable.index["char"] || tokenVec[curIndex].id == keyWordTable.index["float"] || tokenVec[curIndex].id == keyWordTable.index["bool"])) {
        Variable_Declaration();
    } else if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["use"]) {
        UseFunction();
    } else if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        Assignment();
    } else {
        cout << "error when delcare" << endl;
        exit(0);
    }
}


//debug的时候可以调节
void Parse::Logic() {
    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
        curIndex++;
        ArithmeticExpression();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
            curIndex++;
            if(isLogicSign(tokenVec[curIndex])) {
                curIndex++;
                if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
                    ArithmeticExpression();
                    if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                        // GEQ();
                    } else {
                        cout << "error when missing )" << endl;
                        exit(0);
                    }
                } else {
                    cout << "error when missing (" << endl;
                    exit(0);
                }
            }  else {
                cout << "error when is not logic sign" << endl;
                exit(0);
            }
        } else {
            cout  << "error when missing )" << endl;
            exit(0);
        }
    } else if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["!"]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["("]) {
            curIndex++;
            ArithmeticExpression();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index[")"]) {
                //语义动作 ！
                curIndex++;
            } else {
                cout << "error when missing )" << endl;
                exit(0);
            }
        } else {
            cout << "error when missing(" << endl;
            exit(0);
        }
    } else {
        cout << "logic is error" << endl;
        exit(0);
    }
}

void Parse::Else() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["else"]) {
        curIndex++;
        // else 的语义动作
        if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["{"]) {
            curIndex++;
            CompoundStatement();
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == delimiterTable.index["}"]) {
                curIndex++;
            } else {
                cout << "error when missing } in else" << endl;

                exit(0);
            }
        } else {
            cout << "error when missing { in else" << endl;
            exit(0);
        }
    }
}

void Parse::SuffixExpression() {
    if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
        curIndex++;
        Array();
        if(tokenVec[curIndex].type == TokenType::DELIMTER && okenVec[curIndex].id == delimiterTable.index["++"]) {
            curIndex++;
            // 语义动作
        } else if(tokenVec[curIndex].type == TokenType::DELIMTER && okenVec[curIndex].id == delimiterTable.index["--"]) {
            //语义动作
            curIndex++;
        } else {
            cout << "error in for conditon" << endl;
        }
    } else {
        "condition need an identifer" << endl;
    }
}

void Parse::UseFunction() {
    if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index["use"]) {
        curIndex++;
        if(tokenVec[curIndex].type == TokenType::IDENTIFIER) {
            curIndex++;
            if(tokenVec[curIndex].type == TokenType::DELIMTER && tokenVec[curIndex].id == keyWordTable.index["("]) {
                curIndex++;
                if(tokenVec[curIndex].type == TokenType::KEYWORD && tokenVec[curIndex].id == keyWordTable.index[")"]) {
                    curIndex++;
                } else {
                    cout << "error in missing ) when use function" << endl;
                    exit(0);
                }
            } else {
                cout << "error in missing ( when use function" << endl;
                exit(0);
            }
        } else {
            cout << "error in missing identifer when use function" << endl;
            exit(0);
        }
    } else {
        cout << "error in missing use" << endl;
    }
}

void Parse::Assignment() {
    if(tokenVec[curIndex].type == TokenType::IDENTIFIER ) {
        curIndex++;
        Array();
        Assignment_follow();
    } else {
        cout << "error in missing identifer when assignment" << endl;
        exit(0);
    }
}

void Parse::ArithmeticExpression() {

}

bool Parse::isLogicSign(Token token) {
    if(token.type == TokenType::DELIMTER) {
        return token.id == delimiterTable.index["&&"] || token.id == delimiterTable.index["||"] || delimiterTable.index[">"] ||  delimiterTable.index["<"] ||  delimiterTable.index["="] ||  delimiterTable.index[">="] ||  delimiterTable.index["<="] ||  delimiterTable.index["=="] ||  delimiterTable.index["!="];
    }
    return false;
}

void Parse::Mainfunc() {

}

void Parse::Assignment_follow() {

}



