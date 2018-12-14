//
// Created by thankod on 18-12-14.
//

#include "BasicBlock.h"
#include <utility>
vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov)
{
    set<pair<int, string>> leaders;
    vector<BasicBlock> blocks;
    string curFunc;
    for(int i = 0; i < ov.size(); i++) {
        if(ov[i].op == "fundef") {
            if(i != 0) {
                leaders.insert(make_pair(i, curFunc));
            }
            curFunc = ov[i].name1;
        } else if(ov[i].op == "JMP" || ov[i].op == "ifbegin" || ov[i].op == "while"
           || ov[i].op == "WE" || ov[i].op == "else") {
            //紧跟在转向语句后面的语句是入口语句
            leaders.insert(make_pair(i + 1, curFunc));
        } else if(ov[i].op == "LB" || ov[i].op == "ifend" || ov[i].op == "else" || ov[i].op == "while"
                  || ov[i].op == "whileend") {
            //转向语句的目标是入口语句
            leaders.insert(make_pair(i, curFunc));
        } else if(ov[i].op == "funcall" || ov[i].op == "callend" || ov[i].op == "funend") {
            leaders.insert(make_pair(i, curFunc));
        } else if(ov[i].op == "return") {
            leaders.insert(make_pair(i + 1, curFunc));
        }
    }
    int start = 0;
    for(auto end: leaders) {
        blocks.push_back(BasicBlock(ov.begin() + start, ov.begin() + end.first, end.second));
        start = end.first;
    }
    blocks.push_back(BasicBlock(ov.begin() + start, ov.end(), curFunc));
    return blocks;
}
