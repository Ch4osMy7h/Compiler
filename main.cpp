//
// Created by chaosmyth on 2018/11/19.
//

#include "Scanner.h"
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
    Scanner sc(keyWordTable, identiferTable, delimiterTable, intTable, floatTable, charTable, stringTable, tokenVec, symbolTable);
    sc.scan("src.txt");
    sc.showLex();
    //cout << tokenVec.size() << endl;
    symbolTable.print();
    return 0;
}