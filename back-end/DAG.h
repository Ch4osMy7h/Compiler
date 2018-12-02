//
// Created by thankod on 18-12-2.
//

#ifndef COMPILER_DAG_H
#define COMPILER_DAG_H

#include <vector>
#include "../QuadTurple.h"
using namespace std;

struct BasicBlock
{
    vector<QuadTurple> block;
    BasicBlock(vector<QuadTurple> v, int begin, int end): block(v.begin() + begin , v.end() + end) {};
    BasicBlock() {};
};

#endif //COMPILER_DAG_H
