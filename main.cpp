//
// Created by chaosmyth on 2018/11/19.
//

#include "Scanner.h"
#include "front-end/Parse.h"
#include "SymbolTable.h"
#include "back-end/DAG.h"
using namespace std;
SymbolTable symbolTable;

int main(int argc, char* argv[]) {
    /*
     * 词法分析结果的输出
     */
    string srcfileName = "../src.txt";
    string outputLexDir = "../lex.txt";
    string outputQuadDir = "../quad.txt";
    string outputAsmDir = "../asm.txt";
    if(argc != 8) {
        cout << "Invaild filename" << endl;
        return 0;
    } else {
        srcfileName = argv[1];
        outputLexDir = argv[3];
        outputQuadDir = argv[5];
        outputAsmDir = argv[7];
        cout << "srcpath: " << srcfileName << endl;
        cout << "lexPath: " << outputLexDir << endl;
        cout << "quadPath: " << outputQuadDir << endl;
        cout << "targetpath" << outputAsmDir << endl;
    }
    KeyWordTable keyWordTable;
    IdentiferTable identiferTable;
    DelimiterTable delimiterTable;
    IntTable intTable;
    FloatTable floatTable;
    CharTable charTable;
    StringTable stringTable;
    vector<Token> tokenVec;
    vector<QuadTuple> quadVec;
    Scanner sc(keyWordTable, identiferTable, delimiterTable, intTable, floatTable, charTable, stringTable, tokenVec, symbolTable);
    sc.scan(srcfileName);
    sc.showLex(outputLexDir);
//    fclose(stdout);
//    cout << tokenVec.size() << endl;
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
    parse.print(outputQuadDir);

//    symbolTable.print();
//    for (auto &i : symbolTable.symbolTable) {
//        cout << index++  << ":" << endl;
//        for (auto &j : i) {
//            if(j.name == "a") cout << j.type << endl;
//            cout << j.name << " " << parse.toSymTypeName(j.type) << "地址" << j.vall <<" 活跃: " << j.isActive << " 临时:" << j.isTemp << endl;
//        }
//    }
//    test(quadVec);
    //cout << symbolTable.isTempName("t5", "main") << endl;
//    cout << symbolTable.getAddr("a", "main").first << " " << symbolTable.getAddr("a", "main").second << endl;
    return 0;
}