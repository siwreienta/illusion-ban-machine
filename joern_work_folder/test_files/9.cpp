#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>

int main(){
    std::ios_base::sync_with_stdio(0), std::cin.tie(0);
    int n;
    std::cin >> n;
    std::vector<int> tvar(2 * n);
    std::unordered_map<int, int> shlusha;
    bool is_ex_ans = false;
    int a1, d;
    for (int i = 0; i < 2 * n; i++){
        std::cin >> tvar[i];
        if (is_ex_ans) continue;
        shlusha[tvar[i]]++;
        if (shlusha[tvar[i]] == n){
            a1 = tvar[i];
            d = 0;
            is_ex_ans = true;
        }
    }
    if (is_ex_ans){
        std::cout << a1 << " " << d << "\n";
        return 0;
    }
    std::random_device rd;
    std::mt19937_64 gen(rd());
    int x, y, z, k, i_low, i_up, min_pidor;
    while (true){
        k = 1;
        x = y = z = 0;
        while (z == 0){
            x = tvar[gen() % (2 * n)];
            y = tvar[gen() % (2 * n)];
            z = std::abs(x - y);
        }
        min_pidor = std::min(x, y);
        x = min_pidor;
        i_low = i_up = 1;
        while (k < n){
            if (i_low == -1 && i_up == -1) break;
            if (i_low != -1){
                if (shlusha.contains(x - i_low * z)){
                    k++;
                    min_pidor = x - i_low * z;
                    i_low++;
                }
                else{
                    i_low = -1;
                }
            }
            if (k == n) break;
            if (i_up != -1){
                if (shlusha.contains(x + i_up * z)){
                    k++;
                    i_up++;
                }
                else{
                    i_up = -1;
                }
            }
        }
        if (k >= n){
            a1 = min_pidor;
            d = z;
            break;
        }
    }
    std::cout << a1 << " " << d << "\n";
    return 0;
}

