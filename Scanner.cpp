//
// Created by chaosmyth on 2018/11/18.
//

#include "Scanner.h"


void Scanner::scan(string filename) {
    bool comment = false; //是否有注释
    int state = 1;   //当前状态
    int stateBefore; //前一个状态
    char curChar; //当前字符
    int pos = 0;  //当前位置
    string buffer = ""; //识别到的串
    char* over = reinterpret_cast<char *>('#');
    FILE* fp;
    Automatic at; //初始化自动机
    //int comment_cnt = 0;
    //暂时取消
    if( (fp = fopen(filename.c_str(), "r")) == NULL) {
        cout << "open Error" << endl;
    }
    while ((curChar = static_cast<char>(getc(fp))) != '#' ) {
        if(feof(fp)) {
            cout << state << endl;
            if(state == 1 && stateBefore == 1) {}
            else if(state) {
                buffer += curChar;
            } else {
                tokenGenerate(stateBefore, buffer);
            }
            break;
        }
//        cout << 1 << endl;
        if(comment && curChar != '*') {
            continue;
        } else if(curChar == '*' && comment){
            curChar = static_cast<char>(getc(fp));
            if(curChar == '/') {
                if(feof(fp)) {
                    break;
                }
                comment = false;
                continue;
            } else {
                cout << "comment is error" << endl;
                exit(0);
            }
        }
        //跳过空格 换行符 \t 以及回车
        if(curChar == '\n' || curChar == '\t' || curChar == ' ' || curChar == '\r') {
            tokenGenerate(state, buffer);
            if(curChar == '\n') CurLine++;
            pos = 0;
            buffer = "";
            state = 1;
            if(feof(fp)) {
                break;
            }
            continue;
        }
        stateBefore = state;
        state = at.getState(state, curChar);
        //自动机扫描
        if(state == 1 && stateBefore == 1) {}
        else if(state) {
            buffer += curChar;
        } else {
            //判断是否为注释
            if(stateBefore == 43) {
                comment = true;
                if(feof(fp)) {
                    break;
                }
                reset(fp, state, pos, buffer);
                continue;
            }
            tokenGenerate(stateBefore, buffer);
            reset(fp, state, pos, buffer);
        }
    }
    fclose(fp);
}

void Scanner::tokenGenerate(int state_before, string buffer) {
    string code = encoder(state_before);
    Token token;
    if(code == "k") {
        if(keyWordTable.contains(buffer)) {
            token.id = keyWordTable.getID(buffer);
            token.name = buffer;
            token.type = KEYWORD;
            token.line = CurLine;
            tokenVec.push_back(token);
        } else {
            identiferTable.addElement(buffer);
            token.id = identiferTable.getID(buffer);
            token.name = buffer;
            token.type = IDENTIFIER;
            token.line = CurLine;
            tokenVec.push_back(token);
            //symbolTable.entry(buffer, -1, Catgory::None, -1);
        }
    } else if(code == "int") {
        int num = string2num<int >(buffer);
        intTable.addElement(num);
        token.id = intTable.getID(num);
        token.name = buffer;
        token.type = INTCONST;
        token.line = CurLine;
        tokenVec.push_back(token);
//        token.line = CurLine;
    } else if(code == "float") {
        auto num = string2num<double >(buffer);
        token.id = floatTable.getID(num);
        token.name = buffer;
        token.type = FLOATCONST;
        token.line = CurLine;
        tokenVec.push_back(token);
    } else if(code == "char") {
        charTable.addElement(buffer[0]);
        token.id = charTable.getID(buffer[0]);
        token.name = buffer;
        token.type = CHARCONST;
        token.line = CurLine;
        tokenVec.push_back(token);
        token.line = CurLine;
    } else if(code == "string") {
        stringTable.addElement(buffer);
        token.id = stringTable.getID(buffer);
        token.name = buffer;
        token.type = STRCONST;
        token.line = CurLine;
        tokenVec.push_back(token);
        token.line = CurLine;
    } else if(code == "p") {
        token.id = delimiterTable.getID(buffer);
        token.name = buffer;
        token.type = DELIMTER;
        token.line = CurLine;
        tokenVec.push_back(token);
    }
}



//判断进入终止状态时候是什么类型的token
string Scanner::encoder(int state) {
    map<int, string> finalState;
    finalState[2] = "k";
    finalState[3] = "int";
    finalState[7] = "float";
    finalState[16] = "char";
    finalState[17] = "string";
    finalState[19] = "p";
    finalState[13] = "p";
    finalState[4]  = "p";
    finalState[20] = "p";
    finalState[14] = "p";
    finalState[5] = "p";
    finalState[6] = "p";
    finalState[15] = "p";
    finalState[18] = "p";
    finalState[22] = "p";
    finalState[23] = "p";
    finalState[21] = "p";
    finalState[24] = "p";
    for(int i = 25; i <= 42; i++) {
        finalState[i] = "p";
    }
    return finalState[state];
}

//重置自动机扫描头
void Scanner::reset(FILE *fp, int &state, int &pos, string &buffer) {
    pos = 0;
    buffer = "";
    state = 1;
    fseek(fp, -1L, 1);
}

//显示词法分析结果
void Scanner::showLex(string filename) {
    ofstream out;
    out.open(filename, ios::out | ios::trunc);
    if(!out) {
        cout << "Open Lexical File Error" << endl;
    }
    out << "\t\t词法分析结果\t\t" << endl;
    for(auto val : tokenVec) {
        out << setw(10) << val.name << setw(10) << numToName(val.type) << setw(10) << val.id << "   line: " << val.line << endl;
    }
    out.close();
    out.clear();
}


//返回该存入的table
string Scanner::numToName(int num) {
    map<int, string> changeTable;
    changeTable[1] = "identifer";
    changeTable[2] = "int";
    changeTable[3] = "float";
    changeTable[4] = "delimter";
    changeTable[5] = "keyword";
    changeTable[6] = "char";
    changeTable[7] = "string";
    return changeTable[num];
}


Scanner::Scanner(KeyWordTable &kt, IdentiferTable &it, DelimiterTable &dt, IntTable &inta, FloatTable &ft, CharTable &ct,
                 StringTable &st, vector<Token>& tv, SymbolTable& syt) : keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inta), floatTable(ft), charTable(ct), stringTable(st), tokenVec(tv), symbolTable(syt){}


