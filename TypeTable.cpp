//
// Created by chaosmyth on 2018/11/20.
//

#include "TypeTable.h"

DelimiterTable::DelimiterTable() {
    vector<string> delimter = {"+", "-", "*", ",", ".", "(", ")", "[", "]", "{", "}", ";",
                               ">>", ">=", "<<", "<=", "<", "==", "=" ,"++", "+=", "--", "-=",
                               "&=", "&&", "&", "|=", "||", "|", "^", "^=", "!", "!=", "%",
                               "%=", "*", "*=", "/", "/="};
    for(auto ch : delimter) {
        tokenTable.push_back(ch);
        index[ch] = static_cast<int>(tokenTable.size());
    }
}

KeyWordTable::KeyWordTable() {
    vector<string> keyWord = {"int", "main", "void", "if",
                              "else", "char", "double", "float",
                              "while", "for", "do", "break", "struct",
                              "long", "bool", "return", "function", "use"};
    for(auto c : keyWord) {
        tokenTable.push_back(c);
        index[c] = static_cast<int>(tokenTable.size());
    }
}
