//
// Created by thankod on 18-12-2.
//

#include "DAG.h"
#include "../MyUtils.h"

vector<BasicBlock> generateBlocks(vector<QuadTuple>& ov)
{
    set<int> leaders;
    vector<BasicBlock> blocks;
    for(int i = 1; i < ov.size(); i++) {
        if(ov[i].op == "JMP" || ov[i].op == "ifbegin" || ov[i].op == "while" || ov[i].op == "WE" || ov[i].op == "else") {
            //紧跟在转向语句后面的语句是入口语句
            leaders.insert(i + 1);
        } else if(ov[i].op == "LB" || ov[i].op == "ifend" || ov[i].op == "else" || ov[i].op == "while"
            || ov[i].op == "whileend") {
            //转向语句的目标是入口语句
            leaders.insert(i);
        } else if(ov[i].op == "fundef" || ov[i].op == "funcall" || ov[i].op == "callend" || ov[i].op == "funend") {
            leaders.insert(i);
        } else if(ov[i].op == "return") {
            leaders.insert(i + 1);
        }
    }
    int start = 0;
    for(auto end: leaders) {
        blocks.push_back(BasicBlock(ov, start, end));
        start = end;
    }
    blocks.push_back(BasicBlock(ov, start, ov.size()));
    return blocks;
}

void optimizate()
{

}

void optimizateOneBlock(BasicBlock& block)
{
    vector<DAGNode> nodes;
    map<string, int> defineMap;
    //用来在nodes和defineMap里增加新的叶节点的functor
    auto addLeafNode = [&](string op, string name) {
        DAGNode tempNode(nodes.size(), name);
        defineMap[name] = nodes.size();
        nodes.push_back(tempNode);
    };

    //用来处理与四元式的res字段相关事宜的functor
    auto defineRes = [&](string res, int n) {
        if(defineMap.find(res) == defineMap.end()) {
            //res未定义过节点，将res附加在位置为n的附加标记上
            nodes[n].addMarks.push_back(res);
            defineMap[res] = n;
        } else {
            //res定义过节点
            int pos = defineMap[res];
            //先删除已有的附加标记
            if (nodes[pos].left != -1 || nodes[pos].right != -1) {
                //如果不是叶子节点才删除
                auto it = find(nodes[pos].addMarks.begin(), nodes[pos].addMarks.end(), res);
                if(it != nodes[pos].addMarks.end()) {
                    nodes[pos].addMarks.erase(it);
                }
            }
            //再把res附加在位置为n的附加标记上
            nodes[n].addMarks.push_back(res);
            defineMap[res] = n;
        }
    };

    for(auto& qt: block.block) {
        bool defineB = false;
        bool defineC = false;
        int n = 0;
        if(defineMap.find(qt.name1) != defineMap.end()) {
            addLeafNode(qt.op, qt.name1);
            defineB = true;
        }
        if(qt.op == "=") {
            //为赋值语句的时候
            n = defineMap[qt.name1];
            defineRes(qt.res, n);
        } else if (qt.op == "+" || qt.op == "-" || qt.op == "*" || qt.op == "/"
                || qt.op == "<" || qt.op == ">" || qt.op == "==") {
            //如果是双目运算
            if(defineMap.find(qt.name2) != defineMap.end()) {
                //如果第二个操作数没定义过
                addLeafNode(qt.op, qt.name2);
                defineC = true;
            }
            if(isNum(nodes[defineMap[qt.name2]].mainMark) && isNum(nodes[defineMap[qt.name1]].mainMark)) {
                //如果两个操作数对应的节点的主标记都是常数
                if(defineC) {
                    //如果C是处理此次四元式新生成的，则将其删除
                    nodes.pop_back();
                    defineMap.erase(qt.name2);
                }
                if(defineB) {
                    nodes.pop_back();
                    defineMap.erase(qt.name1);
                }
                string ans = calculateNum(qt.op, qt.name1, qt.name2);
                if(defineMap.find(ans) != defineMap.end()) {
                    //如果算出的ans没有定义过，就将它定义为一个叶子节点
                    addLeafNode("", ans);//因为是叶节点，所以操作符为空
                }
                defineRes(qt.res, n);
            } else {
                bool isDefined = false; //如果不是两个常数之间的运算，就要考虑这个运算的结果是否已经有了
                for(auto node: nodes) {
                    if(defineMap[qt.name1] == node.left && defineMap[qt.name2] == node.right & node.mainMark == qt.op) {
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
}

void test()
{
    vector<QuadTuple> quadVec = {QuadTuple("=", "1", " ", "x"),
                                 QuadTuple("LB", "a", " ", " "),
                                 QuadTuple("*", "x", "5", "t1"),
                                 QuadTuple("=", "t1", " ", "r"),
                                 QuadTuple("<", "x", "10", "t2"),
                                 QuadTuple("IF", "t2", " ", " "),
                                 QuadTuple("+", "x", "1", "t3"),
                                 QuadTuple("=", "t3", " ", "x"),
                                 QuadTuple("GT", " ", " ", "a"),
                                 QuadTuple("IE", "1", " ", "x"),
                                 QuadTuple("=", "0", " ", "r"),
                                 QuadTuple("ret", " ", " ", "0")};
    auto a = generateBlocks(quadVec);
    cout << a.size() << endl;
    for(auto s: a) {
        for(auto q: s.block) {
            q.print();
        }
        cout << endl;
    }
}
