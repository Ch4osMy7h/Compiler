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
    vector<string> keyWord = { "auto" , "short" , "int" , "long" , "real" ,
                               "double" , "char" , "struct" , "union" , "enum" , "typedef" ,
                               "const" , "unsigned" , "signed" , "extern" , "register" , "static" ,
                               "volatile" , "void" , "if" , "else" , "switch" , "case" , "for" ,
                               "do" , "while" , "goto" , "continue" , "break" , "default" ,
                               "sizeof" , "return" , "bool" , "cout", "use", "func", "string"};
    for(auto c : keyWord) {
        tokenTable.push_back(c);
        index[c] = static_cast<int>(tokenTable.size());
    }
}
