//
// Created by chaosmyth on 2018/11/19.
//

#include "Scanner.h"
#include "front-end/Parse.h"
using namespace std;

int main(void) {
    /*
     * 词法分析结果的输出
     */
    KeyWordTable keyWordTable;
    IdentiferTable identiferTable;
    DelimiterTable delimiterTable;
    IntTable intTable;
    FloatTable floatTable;
    CharTable charTable;
    StringTable stringTable;
    SymbolTable symbolTable;
    vector<Token> tokenVec;
    vector<QuadTuple> quadVec;
    Scanner sc(keyWordTable, identiferTable, delimiterTable, intTable, floatTable, charTable, stringTable, tokenVec, symbolTable);
    sc.scan("src.txt");
    //sc.showLex();
    //cout << tokenVec.size() << endl;
    //symbolTable.print();
    /*
    Parse::Parse(vector<QuadTurple> &quadVec, vector<Token> &tokenVec, SymbolTable &st, KeyWordTable &kt,
                 IdentiferTable &it, DelimiterTable &dt, IntTable &inTable, FloatTable &ft, CharTable &ct,
                 StringTable &stTable) : quadVec(quadVec), tokenVec(tokenVec), st(st), keyWordTable(kt), identiferTable(it), delimiterTable(dt), intTable(inTable), floatTable(ft), charTable(ct), stringTable(stTable){
        this->curIndex = 0;
        this->curFun = -1;
        this->funCnt = 0;
    }
    */
    Parse parse(quadVec, tokenVec, symbolTable, keyWordTable, identiferTable, delimiterTable, intTable, floatTable, charTable, stringTable);
    parse.parse();
    return 0;
}