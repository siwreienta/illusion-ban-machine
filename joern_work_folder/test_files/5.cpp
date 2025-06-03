#include "optimization.h"
#include <iostream>
#include <vector>

int n, k;

void sss(int idx, int min_el, std::vector<int> &cur){
    if (idx == k){
        for (auto &x : cur){
            writeInt(x);
            writeChar(' ');
        }
        writeChar('\n');
        return;
    }
    for (int i = min_el; i <= n; i++){
        cur[idx] = i;
        sss(idx + 1, i + 1, cur);
    }
}

int main(){
    n = readInt();
    k = readInt();
    std::vector<int> cur(k, 0);
    sss(0, 1, cur);
    return 0;
}