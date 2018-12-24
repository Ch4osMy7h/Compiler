//
// Created by thankod on 18-12-15.
//

#ifndef COMPILER_ASSEMBLY_H
#define COMPILER_ASSEMBLY_H
#include "BasicBlock.h"
#include <array>
struct Instruction
{
    string label;
    string op;
    string name1;
    string name2;
    Instruction(string op, string name1, string name2, string label): op(op), name1(name1), name2(name2), label(label)  {}
    Instruction(string op, string name1, string name2): op(op), name1(name1), name2(name2), label()  {}
    Instruction() = default;
    void print(ofstream& outFirst) {
        outFirst.setf(std::ios::left);
        outFirst << setw(8) << label;
        if(name2.empty())
            outFirst << setw(8) << op << setw(8) << name1 << endl;
        else
            outFirst << setw(8) << op << setw(17) << name1 + "," << setw(8) << name2 << endl;
    }
};

//用来指示存放位置的结构体
struct Position
{
    string pos;
    bool isMem;
    Position(string pos, bool isMem): pos(pos), isMem(isMem) {};
    Position() = default;
};

vector<Instruction> geneASM(vector<BasicBlock>& blocks);

void activeInfo(BasicBlock& block);

#endif //COMPILER_ASSEMBLY_H
