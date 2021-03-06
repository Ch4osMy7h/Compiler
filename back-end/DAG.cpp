//
// Created by thankod on 18-12-2.
//

#include "DAG.h"
#include "../MyUtils.h"
#include "BasicBlock.h"
#include "Assembly.h"

vector<QuadTuple> DAGToQuadTuple(vector<DAGNode> nodes, string curFunc)
{

    auto getValue = [&](int pos) {
        return nodes[pos].mainMark.empty() ? nodes[pos].addMarks[0] : nodes[pos].mainMark;
    };
    vector<QuadTuple> ans;
    for(auto node: nodes) {
        if(node.isDeleted) {
            continue;
        }
        if(node.left == -1 && node.right == -1) {
            for(auto addMark: node.addMarks) {
                if(!isTempName(addMark, curFunc)) {
                    ans.emplace_back("=", node.mainMark, "__", addMark);
                }
            }
        } else {
            ans.emplace_back(node.op, getValue(node.left), getValue(node.right), node.addMarks[0]);
            for(int i = 1; i < node.addMarks.size(); i++) {
                if(!isTempName(node.addMarks[i], curFunc)) {
                    ans.emplace_back("=", node.addMarks[0], "__", node.addMarks[i]);
                }
            }
        }
    }
    return ans;
}

vector<DAGNode> optimizeOneBlock(vector<QuadTuple> quadVector, string curFunc)
{
    vector<DAGNode> nodes;
    map<string, int> defineMap;
    //用来在nodes和defineMap里增加新的叶节点的函数, 返回插入的元素的位置的迭代器
    auto addLeafNode = [&](string name) {
        DAGNode tempNode(nodes.size(), name);
        defineMap[name] = nodes.size();
        nodes.push_back(tempNode);
        return nodes.size() - 1;
    };

    //用来处理与四元式的res字段相关事宜的函数
    auto defineRes = [&](string res, int n) {
        if(defineMap.find(res) != defineMap.end()) {
            //res未定义过节点，将res附加在位置为n的附加标记上
            int pos = defineMap[res];
            //先删除已有的附加标记
            if (true) {
                //如果不是叶子节点才删除
                auto it = find(nodes[pos].addMarks.begin(), nodes[pos].addMarks.end(), res);
                if(it != nodes[pos].addMarks.end()) {
                    nodes[pos].addMarks.erase(it);
                }
            }
        }
        nodes[n].addMarks.push_back(res);
        defineMap[res] = n;
        if(!isTempName(res, curFunc)) {
            swap(nodes[n].addMarks[0], nodes[n].addMarks[nodes[n].addMarks.size() - 1]);
        }
    };

    for(auto& qt: quadVector) {
        int positionOfB = -1;
        int positionOfC = -1;

        int n = 0;//用来记录赋值语句赋值内容的位置
        if(defineMap.find(qt.name1) == defineMap.end()) {
            positionOfB = addLeafNode(qt.name1);
        }
        if(qt.op == "=") {
            //为赋值语句的时候
            n = defineMap[qt.name1];
            defineRes(qt.res, n);
        } else if (qt.op == "+" || qt.op == "-" || qt.op == "*" || qt.op == "/"
                   || qt.op == "<" || qt.op == ">" || qt.op == "==") {
            //如果是双目运算
            if(defineMap.find(qt.name2) == defineMap.end()) {
                //如果第二个操作数没定义过
                positionOfC = addLeafNode(qt.name2);
            }
            if(isNum(nodes[defineMap[qt.name2]].mainMark) && isNum(nodes[defineMap[qt.name1]].mainMark)) {
                //如果两个操作数对应的节点的主标记都是常数
                string ans = calculateNum(qt.op, nodes[defineMap[qt.name1]].mainMark,
                                          nodes[defineMap[qt.name2]].mainMark);
                if(defineMap.find(ans) == defineMap.end()) {
                    //如果算出的ans没有定义过，就将它定义为一个叶子节点
                    n = addLeafNode(ans);
                } else {
                    n = defineMap[ans];
                }
                if(positionOfC != -1) {
                    //如果C是处理此次四元式新生成的，则将其删除
                    defineMap.erase(nodes[positionOfC].mainMark);
                    nodes[positionOfC].isDeleted = true;
                }
                if(positionOfB != -1) {
                    defineMap.erase(nodes[positionOfB].mainMark);
                    nodes[positionOfB].isDeleted = true;
                }
                defineRes(qt.res, n);
            } else {
                bool isDefined = false; //如果不是两个常数之间的运算，就要考虑这个运算的结果是否已经有了
                for(auto node: nodes) {
                    if(defineMap[qt.name1] == node.left && defineMap[qt.name2] == node.right && node.op == qt.op
                       && !node.isDeleted) {
                        n = node.label;
                        isDefined = true;
                        break;
                    }
                }
                if(!isDefined) {
                    //如果这个之前没计算过，要建立一个新的结点，连接两个操作数
                    DAGNode tempNode(nodes.size(), qt.op, defineMap[qt.name1], defineMap[qt.name2]);
                    n = nodes.size();
                    nodes.push_back(tempNode);
                }
                defineRes(qt.res, n);
            }
        }
    }
    return nodes;
}

void optimize(vector<BasicBlock>& blocks)
{
    auto connect = [](vector<QuadTuple> a, vector<QuadTuple> b, vector<QuadTuple> c) {
        a.insert(a.end(), b.begin(), b.end());
        a.insert(a.end(), c.begin(), c.end());
        return a;
    };

    for(auto& block: blocks) {
        if(block.start == -1 || block.finish == -1) {
            continue;
        }
        block.block = connect(block.exprVectorBefore(),
                DAGToQuadTuple(optimizeOneBlock(block.exprVector(), block.curFun), block.curFun),
                block.exprVectorAfter());
    }
}




void test(vector<QuadTuple> quadVec, string fileNameBlcok, string fileNameInst)
{
    ofstream out;
    out.open(fileNameBlcok, ios::out | ios::trunc);
    auto blocks = generateBlocks(quadVec);
    optimize(blocks);
    out << blocks.size() << endl;
    int i = 0;
    for(auto& block: blocks) {
        out << "第" << i++ << "个基本块:" << endl;
        out << "所属函数名：" << block.curFun << endl;
        activeInfo(block);
        for(auto qt: block.block) {
            qt.printWithAct(out);
        }
    }
    out.close();


    ofstream outInst;
    outInst.open(fileNameInst, ios::out | ios::trunc);
    auto insts = geneASM(blocks);;
    for(auto inst: insts) {
        inst.print(outInst);
    }
    outInst.close();
}
