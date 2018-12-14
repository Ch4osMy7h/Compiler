//
// Created by thankod on 18-12-14.
//

#ifndef COMPILER_BASICBLOCK_H
#define COMPILER_BASICBLOCK_H
#include <vector>
#include <set>
#include "../QuadTuple.h"
#include "../MyUtils.h"
struct BasicBlock
{
    vector<QuadTuple> block;
    int start;//除去控制和声明语句后的开头和结尾(只有DAG优化阶段有用)
    int finish;
    string curFun;
    BasicBlock(vector<QuadTuple>::iterator begin, vector<QuadTuple>::iterator end, string curFun):
                block(begin, end), curFun(curFun) {
        start = -1;
        finish = -1;
        for(int i = 0; i < block.size(); i++) {
            if(isOperator(block[i].op)) {
                //寻找第一个表达式的位置
                start = i;
                break;
            }
        }
        if(start != -1) {
            for(int i = block.size() - 1; i >= start; i--) {
                if(isOperator(block[i].op)) {
                    //寻找最后一个表达式的位置
                    finish = i;
                    break;
                }
            }
        }

    };
    BasicBlock() = default;
    vector<QuadTuple> exprVector() { return vector<QuadTuple>(block.begin() + start, block.begin() + finish + 1); }
    vector<QuadTuple> exprVectorBefore() { return vector<QuadTuple>(block.begin(), block.begin() + start); }
    vector<QuadTuple> exprVectorAfter() { return vector<QuadTuple>(block.begin() + finish + 1, block.end()); }

};

vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov);

#endif //COMPILER_BASICBLOCK_H
