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
    BasicBlock(vector<QuadTuple>& v, int begin, int end): block(v.begin() + begin , v.begin() + end) {};
    BasicBlock() = default;
};

vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov);

void test();

struct DAGNode
{
    int label;
    string mainMark;//主标记，叶子节点时为值，非叶子节点时为运算符
    vector<string> addMarks;
    int left;
    int right;
    //用于构造叶结点的构造函数
    DAGNode(int label, string mainMark): label(label), mainMark(mainMark), left(-1),
                                                    right(-1), addMarks() {};
    //用于构造非叶节点的构造函数
    DAGNode(int label, string mainMark, int left, int right): label(label), mainMark(mainMark), left(left),
                                                    right(right), addMarks() {};
    DAGNode() = default;

};

struct DAG
{
    
};

#endif //COMPILER_DAG_H
