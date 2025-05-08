#include "optimization.h"
#include <iostream>
#include <vector>

int n, k;

void sss(int idx, int max_el, std::vector<int> &cur){
    if (idx == k){
        for (auto &x : cur){
            writeInt(x);
            writeChar(' ');
        }
        writeChar('\n');
        return;
    }
    for (int i = max_el; i > 0; i--){
        if (i < k - idx){
            return;
        }
        cur[idx] = i;
        sss(idx + 1, i - 1, cur);
    }
}

int main(){
    n = readInt();
    k = readInt();
    std::vector<int> cur(k, 0);
    sss(0, n, cur);
    return 0;
}