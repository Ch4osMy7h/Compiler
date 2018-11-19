//
// Created by chaosmyth on 2018/11/18.
//

#include "Token.h"


Token::Token() {
    this->type = KEYWORD;
    this->name = "";
    this->id = -1;
}

Token::Token(TokenType type, string name, int id) {
    this->type = type;
    this->name = name;
    this->id = id;
}

void Token::print() {
    cout << type << "\t" << name << "\t" << id;
}

