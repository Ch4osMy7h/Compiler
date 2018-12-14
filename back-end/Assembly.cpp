//
// Created by thankod on 18-12-15.
//

#include "Assembly.h"
#include "BasicBlock.h"

void activeInfo(BasicBlock& block)
{
    for(int i = block.block.size() - 1; i >= 0; i--) {
        if(!isOperator(block.block[i].op)) {
            continue;
        }
        block.block[i].resAct = isActive(block.block[i].res, block.curFun);
        setActive(block.block[i].res, block.curFun, false);
        block.block[i].name1 = isActive(block.block[i].name1, block.curFun);
        block.block[i].name2 = isActive(block.block[i].name2, block.curFun);
        setActive(block.block[i].name1, block.curFun, true);
        setActive(block.block[i].name2, block.curFun, true);
    }
}