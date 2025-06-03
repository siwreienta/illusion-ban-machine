#include "optimization.h"
#include <iostream>
#include <vector>

int n, k;
std::vector<int> cur;

void sss(int idx, int min_el){
    if (idx == k){
        for (auto &x : cur){
            writeInt(x);
            writeChar(' ');
        }
        writeChar('\n');
        return;
    }
    if (n - min_el + 1 < k - idx){
        return;
    }
    for (int i = min_el; i <= n; i++){
        cur[idx] = i;
        sss(idx + 1, i + 1);
    }
}

int main(){
    k = readInt();
    n = readInt();
    cur.resize(k);
    sss(0, 1);
    return 0;
}