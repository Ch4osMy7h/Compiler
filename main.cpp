//
// Created by chaosmyth on 2018/11/19.
//

#include "Scanner.h"
using namespace std;

int main(void) {
    /*
     * 词法分析结果的输出
     */
    Scanner sc;
    sc.scan("src.txt");
    sc.showLex();
    return 0;
}