//
// Created by thankod on 18-12-2.
//

#include "DAG.h"

vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov)
{
    set<int> leaders;
    leaders.insert(0);
    for(int i = 1; i < ov.size(); i++) {
        if(ov[i].op == "JMP" || ov[i].op == "IF" || ov[i].op == "DO" || ov[i].op == "WE" || ov[i].op == "EL") {
            //紧跟在转向语句后面的语句是入口语句
            leaders.insert(i + 1);
        } else if(ov[i].op == "LB" || ov[i].op == "IE" || ov[i].op == "EL" || ov[i].op == "IE" || ov[i].op == "WH"
            || ov[i].op == "WE") {
            //转向语句的目标是入口语句
            leaders.insert(i + 1);
        }
    }

}
