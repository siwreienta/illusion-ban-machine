#include <iostream>
#include <vector>
#include "optimization.h"

int main(){
    int n = readInt();
    std::vector<int> dp(n + 1, 1e6);
    std::vector<int> check(n + 1, -1);
    dp[n] = 0;
    for (int i = n; i > 1; i--){
        if (dp[i - 1] > dp[i] + 1){
            dp[i - 1] = dp[i] + 1;
            check[i - 1] = i;
        }
        if (i % 2 == 0 && dp[i / 2] > dp[i] + 1){
            dp[i / 2] = std::min(dp[i / 2], dp[i] + 1);
            check[i / 2] = i;
        }
        writeWord("HUI\n");
    }
    writeInt(dp[1]);
    writeChar('\n');
    int current_index = 1;
    while (current_index != -1){
        writeInt(current_index);
        writeChar(' ');
        current_index = check[current_index];
    }
    writeWord("HUI\n");
    return 0;
}