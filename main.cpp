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
    string outputDAGDir;
    string outputInstDir;
    string outputTableDir;
    if(argc != 12) {
        cout << "Invaild filename" << endl;
        return 0;
    } else {
        srcfileName = argv[1];
        outputLexDir = argv[3];
        outputQuadDir = argv[5];
        outputDAGDir = argv[7];
        outputInstDir = argv[9];
        outputTableDir = argv[11];
        cout << "srcpath: " << srcfileName << endl;
        cout << "lexPath: " << outputLexDir << endl;
        cout << "quadPath: " << outputQuadDir << endl;
        cout << "targetpath: " << outputAsmDir << endl;
        cout << "outputDAGDir: " << outputDAGDir << endl;
        cout << "outputInstDir: " << outputInstDir << endl;
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
    test(quadVec, outputDAGDir, outputInstDir);
    //symbolTable.print();
    int index = 0;
    int cur;
    ofstream out;
    out.open(outputTableDir, ios::out | ios::trunc);
    for (auto &i : symbolTable.symbolTable) {
        out << index++  << ":" << endl;
        cur = 0;
        for (auto &j : i) {
            out  << j.name << " " << parse.toSymTypeName(j.type) <<  " " <<  parse.toCatName(j.cat) << " " << j.vall<< endl;
        }
    }
    out << "数组表" << endl;
    out << "Low " <<"up " << "clen " << "type " << endl;
    for (auto &i : symbolTable.arrayTableVec) {
        out << i.low << " " << i.up << " " << i.clen << " " << parse.toSymTypeName(i.type) << endl;
    }


    out << "函数表" << endl;
//    out << "Low " <<"up " << "clen " << "type " << endl;
//    for (auto &i : symbolTable.functionTableVec) {
//        out << i.paramNum << " " << i. << " " << i.clen << " " << parse.toSymTypeName(i.type) << endl;
//    }
    out.close();
//    test(quadVec);
    //cout << symbolTable.isTempName("t5", "main") << endl;
//    cout << symbolTable.getAddr("a", "main").first << " " << symbolTable.getAddr("a", "main").second << endl;
    return 0;
}