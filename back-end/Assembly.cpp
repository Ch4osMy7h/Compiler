//
// Created by thankod on 18-12-15.
//

#include "Assembly.h"
#include <stack>
#include "../MyUtils.h"

void activeInfo(BasicBlock &block)
{
    for(int i = block.block.size() - 1; i >= 0; i--) {
        if(!isOperator(block.block[i].op)) {
            continue;
        }
        setActive(block.block[i].res, block.curFun, !isTempName(block.block[i].res, block.curFun));
        setActive(block.block[i].name1, block.curFun, !isTempName(block.block[i].name1, block.curFun));
        setActive(block.block[i].name2, block.curFun, !isTempName(block.block[i].name2, block.curFun));
    }

    for(int i = block.block.size() - 1; i >= 0; i--) {
        if(!isOperator(block.block[i].op)) {
            continue;
        }
        block.block[i].resAct = isActive(block.block[i].res, block.curFun);
        setActive(block.block[i].res, block.curFun, false);
        block.block[i].name1Act = isActive(block.block[i].name1, block.curFun);
        block.block[i].name2Act = isActive(block.block[i].name2, block.curFun);
        setActive(block.block[i].name1, block.curFun, true);
        setActive(block.block[i].name2, block.curFun, true);
    }

    //保证符号表内活跃信息和四元式内一致
    for(int i = 0; i < block.block.size(); i++) {
        if(!isOperator(block.block[i].op)) {
            continue;
        }
        setActive(block.block[i].name1, block.curFun, block.block[i].name1Act);
        setActive(block.block[i].name2, block.curFun, block.block[i].name2Act);
        setActive(block.block[i].res, block.curFun, block.block[i].resAct);
    }

}


vector<Instruction> geneASM(vector<BasicBlock> &blocks)
{
    //没有AL是因为为了方便，AX寄存器保留作为乘除法运算单元与返回值保存寄存器
    map<string, string> RDL = {make_pair("BL", ""),
                               make_pair("CL", ""),
                               make_pair("DL", "")};
    map<string, Position> posMap;
    vector<Instruction> instVec;
    instVec.emplace_back("SEGMENT", "", "", "DSEG");
    instVec.emplace_back("DB", "0", "", "TEMP");//用来存临时变量
    instVec.emplace_back("ENDS", "", "", "DSEG");
    instVec.emplace_back("SEGMENT", "", "", "SSEG");
    instVec.emplace_back("DB", "100   DUP(0)", "", "STACK");
    instVec.emplace_back("ENDS", "", "", "SSEG");
    instVec.emplace_back("SEGMENT", "", "", "CSEG");
    instVec.emplace_back("ASSUME", "CS:CSEG", "", "");
    instVec.emplace_back("ASSUME", "DS:DSEG", "", "");
    instVec.emplace_back("ASSUME", "SS:SSEG", "", "");

    stack<int> sem;//用来记录返填信息的
    int labelNow = 0; //用来记录当前label标号

    //获得某个变量的储存位置
    auto getAddr = [&](string name, string curFun) -> string {
        if(posMap[name].isMem) {
            auto addr = getAddrFromTable(name, curFun);
            if(addr.first) {
                //如果此函数里没有此变量，是全局变量
                return "STACK[" + to_string(addr.second) + "]";
            } else {
                return "[BP+" + to_string(addr.second) + "]";
            }
        } else {
            return posMap[name].pos;
        }
    };

    cout << "nimasile"<< getAddrFromTable("y", "main").second << endl;

        //找到一个空的通用寄存器
    auto findEmpty = [&]() -> string {
        if (RDL["BL"].empty())
            return "BL";
        if (RDL["CL"].empty())
            return "CL";
        if (RDL["DL"].empty())
            return "DL";
        return "";
    };

    //跳转出去前和基本块结束后，要保存当前寄存器所有的值
    auto saveR = [&](string curFun) {
        for(auto pair: RDL) {
            if(!pair.second.empty()) {
                if(isActive(pair.second, curFun)) {
                    //保存活跃变量
                    posMap[pair.second].isMem = true;
                    instVec.emplace_back("MOV", getAddr(pair.second, curFun), pair.first);
                    RDL[pair.first].clear();
                } else {
                    RDL[pair.first].clear();
                }
            }
        }
    };

    //运算四元式寄存器分配
    auto getR = [&](QuadTuple qt, string& R, string& B, string& C, string curFun) {
        if(posMap.find(qt.name1) != posMap.end() && !posMap[qt.name1].isMem) {
            //有寄存器存B的值，主动释放
            if(qt.name1Act && (!posMap[qt.name1].isMem || qt.name1 != qt.res)) {
                //如果B不在内存里或B和目标RES不是相同的，而且B活跃
                string emptyR = findEmpty();
                if(!emptyR.empty()) {
                    //如果有空寄存器，就将当前存放B的寄存器的值移到那个空寄存器里
                    instVec.emplace_back("MOV", emptyR, posMap[qt.name1].pos);
                    RDL[emptyR] = qt.name1;//新找到的寄存器里放B
                    R = getAddr(qt.name1, curFun); B = emptyR;
                    if(qt.op != "=") C = getAddr(qt.name2, curFun);
                    RDL[posMap[qt.name1].pos].clear();//此时map里的值还是之前B的储存地，将其清空
                    posMap[qt.name1] = Position(emptyR, false);
                } else {
                    //如果没有，就加入内存里
                    posMap[qt.name1].isMem = true;//先将其的储存地设为内存，让下一行的getAddr能获取它的内存地址
                    instVec.emplace_back("MOV", getAddr(qt.name1, curFun), posMap[qt.name1].pos);
                    R = posMap[qt.name1].pos; B = getAddr(qt.name1, curFun);
                    if(qt.op != "=") C = getAddr(qt.name2, curFun);
                    RDL[posMap[qt.name1].pos].clear();
                    posMap[qt.name1] = Position(qt.name1, true);
                }
            } else {
                //否则（比如B不活跃）
                R = getAddr(qt.name1, curFun); B = getAddr(qt.name1, curFun);
                if(qt.op != "=") C = getAddr(qt.name2, curFun);
                RDL[posMap[qt.name1].pos].clear();
                posMap[qt.name1] = Position(qt.name1, true);
            }
        } else if((qt.op == "+" || qt.op == "*" || qt.op == "==") &&
                    posMap.find(qt.name2) != posMap.end() && !posMap[qt.name2].isMem) {
            //如果可交换，且有寄存器存C的值，将C和B交换后执行同样操作
            swap(qt.name1, qt.name2);
            swap(qt.name1Act, qt.name2Act);
            if(qt.name1Act && (!posMap[qt.name1].isMem || qt.name1 != qt.res)) {
                //如果B不在内存里或B和目标RES不是相同的，而且B活跃
                string emptyR = findEmpty();
                if(!emptyR.empty()) {
                    //如果有空寄存器，就将当前存放B的寄存器的值移到那个空寄存器里
                    instVec.emplace_back("MOV", emptyR, posMap[qt.name1].pos);
                    RDL[emptyR] = qt.name1;//新找到的寄存器里放B
                    R = getAddr(qt.name1, curFun); B = emptyR;
                    if(qt.op != "=") C = getAddr(qt.name2, curFun);
                    RDL[posMap[qt.name1].pos].clear();//此时map里的值还是之前B的储存地，将其清空
                    posMap[qt.name1] = Position(emptyR, false);
                } else {
                    //如果没有，就加入内存里
                    posMap[qt.name1].isMem = true;//先将其的储存地设为内存，让下一行的getAddr能获取它的内存地址
                    instVec.emplace_back("MOV", getAddr(qt.name1, curFun), posMap[qt.name1].pos);
                    R = posMap[qt.name1].pos; B = getAddr(qt.name1, curFun);
                    if(qt.op != "=") C = getAddr(qt.name2, curFun);
                    RDL[posMap[qt.name1].pos].clear();
                    posMap[qt.name1] = Position(qt.name1, true);
                }
            } else {
                //否则（比如B不活跃）
                R = getAddr(qt.name1, curFun); B = getAddr(qt.name1, curFun);
                if(qt.op != "=") C = getAddr(qt.name2, curFun);
                RDL[posMap[qt.name1].pos].clear();
                posMap[qt.name1] = Position(qt.name1, true);
            }
        } else if(!findEmpty().empty()) {
            //如果以上两个条件都不满足，则选空闲者
            string emptyR = findEmpty();
            R = emptyR; B = getAddr(qt.name1, curFun);
            if(qt.op != "=") C = getAddr(qt.name2, curFun);
        } else {
            //如果前三个情况都不满足，只能强制剥夺了，优先剥夺寄存器里不活跃的变量
            string obj = "";
            string objPos;
            for(auto pair: RDL) {
                if(posMap.find(pair.second) != posMap.end() && !posMap[pair.second].isMem && !isActive(pair.second, curFun)) {
                    obj = pair.second;
                    objPos = pair.first;
                    break;
                }
            }
            //如果没有不活跃的变量，只能随便剥夺一个了
            if(obj.empty()) {
                for(auto pair: RDL) {
                    if(posMap.find(pair.second) != posMap.end()) {
                        obj = pair.second;
                        objPos = pair.first;
                        break;
                    }
                }
            }
            instVec.emplace_back("MOV", obj, objPos);
            RDL[objPos].clear();
            posMap[obj] = Position(obj, true);
            R = objPos; B = getAddr(qt.name1, curFun);
            if(qt.op != "=") C = getAddr(qt.name2, curFun);
        }
    };

    for(auto block: blocks) {
        string cmpType;//存放生成cmp指令对应的是小于还是等于还是大于，用于下一次编写if和do的跳转指令
        saveR(block.curFun);
        activeInfo(block);
        posMap.clear();
        for(auto qt: block.block) {
            if(isOperator(qt.op)) {
                //一个基本块开始前，先基本块内所有的变量都设为储存器变量
                posMap[qt.name1] = Position(qt.name1, true);
                if(qt.name2 != "__") posMap[qt.name2] = Position(qt.name2, true);
                posMap[qt.res] = Position(qt.res, true);
            }
        }
        for(auto qt: block.block) {
            if(isOperator(qt.op)) {
                string R, B, C;
                getR(qt, R, B, C, block.curFun);
                if(qt.op == "=") {
                    //如果是赋值语句
                    if(R != B) instVec.emplace_back("MOV", R, B);
                    posMap[qt.res] = Position(R, false);
                    RDL[R] = qt.res;
                } else {
                    if(R != B) instVec.emplace_back("MOV", R, B);
                    if(qt.op == "*") {
                        instVec.emplace_back("MOV", "AL", B);
                        instVec.emplace_back("MOV", "AH", "0");
                        if(isNum(C)) {
                            //MUL不能接立即数，因此需要将此立即数存起来
                            instVec.emplace_back("MOV", "TEMP", C);
                            instVec.emplace_back("MUL", "TEMP", "");
                        } else {
                            instVec.emplace_back("MUL", C, "");
                        }
                        instVec.emplace_back("MOV", R, "AL");
                    } else if(qt.op == "/") {
                        instVec.emplace_back("MOV", "AL", B);
                        instVec.emplace_back("MOV", "AH", "0");
                        if(isNum(C)) {
                            //DIV不能接立即数，因此需要将此立即数存起来
                            instVec.emplace_back("MOV", "TEMP", C);
                            instVec.emplace_back("DIV", "TEMP", "");
                        } else {
                            instVec.emplace_back("DIV", C, "");
                        }
                        instVec.emplace_back("MOV", R, "AL");
                    } else if(qt.op == "+") {
                        instVec.emplace_back("ADD", R, C);
                    } else if(qt.op == "-") {
                        instVec.emplace_back("SUB", R, C);
                    } else if(qt.op == "==" || qt.op == ">" || qt.op == "<") {
                        instVec.emplace_back("CMP", R, C);
                        cmpType = qt.op;
                    }
                    posMap[qt.res] = Position(R, false);
                    RDL[R] = qt.res;
                }
            } else if (qt.op == "ifbegin") {
                //如果是跳转语句，因为跳转语句需要判断的对象必然刚计算完毕，故可以用标志转移指令进行跳转
                saveR(block.curFun);
                if(cmpType == "<") {
                    instVec.emplace_back("JAE", "", "");//如果B实际上大于等于C，那么就跳到else
                } else if(cmpType == ">") {
                    instVec.emplace_back("JBE", "", "");//如果B实际上小于等于C，那么就跳到else
                } else if(cmpType == "==") {
                    instVec.emplace_back("JNE", "", "");//如果B实际上不等于C，那么就跳到else
                }
                sem.push(instVec.size() - 1);
            } else if (qt.op == "else") {
                //同时为跳转指令和跳转目标
                saveR(block.curFun);
                instVec.emplace_back("JMP", "", "");//无条件跳到endif之后，需要返填
                int pos = sem.top();//获取到需要返填的位置
                sem.pop();
                sem.push(instVec.size() - 1);
                instVec[pos].name1 = "LABEL" + to_string(labelNow);//将JMP的下一条返填进去
                instVec.emplace_back("", "", "", "LABEL" + to_string(labelNow) + ":");
                labelNow++;
            } else if (qt.op == "ifend") {
                saveR(block.curFun);
                int pos = sem.top();
                sem.pop();
                instVec[pos].name1 = "LABEL" + to_string(labelNow);//将JMP的下一条返填进去
                instVec.emplace_back("", "", "", "LABEL" + to_string(labelNow)  + ":");
                labelNow++;
            } else if (qt.op == "while") {
                //为whileend以后回到开始重新判断条件保存下一条语句位置
                sem.push(instVec.size());
                instVec.emplace_back("", "", "", "LABEL" + to_string(labelNow)  + ":");
                labelNow++;
            } else if (qt.op == "do") {
                saveR(block.curFun);
                if(cmpType == "<") {
                    instVec.emplace_back("JAE", "", "");//如果B实际上大于等于C，那么就跳到whileend
                } else if(cmpType == ">") {
                    instVec.emplace_back("JBE", "", "");//如果B实际上小于等于C，那么就跳到whileend
                } else if(cmpType == "==") {
                    instVec.emplace_back("JNE", "", "");//如果B实际上不等于C，那么就跳到whileend
                }
                sem.push(instVec.size() - 1);
            } else if (qt.op == "whileend") {
                saveR(block.curFun);
                instVec.emplace_back("JMP", "", "");//跳回判断部分的语句
                int temp = instVec.size() - 1;//上一行加入的语句的位置
                int pos = sem.top();
                sem.pop();
                instVec[pos].name1 = "LABEL" + to_string(labelNow);//将JMP的下一条返填进去
                instVec.emplace_back("", "", "", "LABEL" + to_string(labelNow)  + ":");
                labelNow++;
                pos = sem.top();
                sem.pop();
                instVec[temp].name1 = instVec[pos].label;
            } else if(qt.op == "fundef") {
                if(block.curFun == "main") {
                    //遇到主函数的时候，将其设为程序的入口
                    instVec.emplace_back("" , "", "", "MAIN:");
                    instVec.emplace_back("MOV", "AX", "DSEG");
                    instVec.emplace_back("MOV", "DS", "AX");
                    instVec.emplace_back("MOV", "AX", "SSEG");
                    instVec.emplace_back("MOV", "SS", "AX");
                } else {
                    instVec.emplace_back("PROC", "NEAR", "", block.curFun);
                }
            } else if(qt.op == "funend") {
                if(block.curFun == "main") {
                    instVec.emplace_back("ENDS", "", "", "CSEG");
                    instVec.emplace_back("END", "MAIN", "");
                } else {
                    instVec.emplace_back("ENDP", "", "", block.curFun);

                }
            } else if(qt.op == "return") {
                if(block.curFun == "main") {
                    instVec.emplace_back("MOV", "AH", "4CH");
                    instVec.emplace_back("INT", "21H", "");
                    RDL["BL"].clear();
                    RDL["CL"].clear();
                    RDL["DL"].clear();
                } else {
                    instVec.emplace_back("MOV", "AL", posMap[qt.name1].pos);//将返回值暂存在AX中
                    instVec.emplace_back("RET", "", "");
                }
            }
        }
    }
    return instVec;
}