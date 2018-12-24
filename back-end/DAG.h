//
// Created by thankod on 18-12-2.
//

#ifndef COMPILER_DAG_H
#define COMPILER_DAG_H

#include <vector>
#include <set>
#include "../QuadTuple.h"
#include "BasicBlock.h"
#include <fstream>
using namespace std;


void test(vector<QuadTuple> quadVec, string fileNameDAG, string fileNameInst);

struct DAGNode
{
    int label;
    string op;
    string mainMark;
    vector<string> addMarks;
    int left;
    int right;
    bool isDeleted;
    //用于构造叶结点的构造函数
    DAGNode(int label, string mainMark): label(label), mainMark(mainMark), left(-1),
                                                    right(-1), addMarks(), isDeleted(false) {};
    //用于构造非叶节点的构造函数
    DAGNode(int label, string op, int left, int right): label(label), mainMark(""), op(op), left(left),
                                                    right(right), addMarks(), isDeleted(false){};
    DAGNode() = default;

};

#endif //COMPILER_DAG_H
