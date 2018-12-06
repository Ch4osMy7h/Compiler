//
// Created by thankod on 18-12-2.
//

#ifndef COMPILER_DAG_H
#define COMPILER_DAG_H

#include <vector>
#include <set>
#include "../QuadTuple.h"
using namespace std;

struct BasicBlock
{
    vector<QuadTuple> block;
    BasicBlock(vector<QuadTuple> v, int begin, int end): block(v.begin() + begin , v.end() + end) {};
    BasicBlock() = default;
};

vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov);


struct Node
{
    int label;
    string op;
    string mainMark;
    vector<string> addMark;
};

struct DAG
{
    
};

#endif //COMPILER_DAG_H
