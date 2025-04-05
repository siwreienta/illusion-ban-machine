    #include "optimization.h"
    #include <vector>

    std::vector<int> result;
    bool is_cycle = false;
    int answer, last;

    void dfs(int v, std::vector<std::vector<int>> &g, std::vector<int> &used, std::vector<int> &prev){
        if (is_cycle){
            return;
        }
        used[v] = 1;
        for (auto u : g[v]){
            if (is_cycle) return;
            if (used[u] == 0){
                prev[u] = v;
                dfs(u, g, used, prev);
            }
            else if (used[u] == 2){
                continue;
            }
            else{
                is_cycle = true;
                answer = u;
                last = v;
                return;
            }
        }
        used[v] = 2;
    }

    int main(){
        int n = readInt(), m = readInt(), b, e;
        std::vector<std::vector<int>> g(n);
        for (int i = 0; i < m; i++){
            b = readInt() - 1;
            e = readInt() - 1;
            g[b].push_back(e);
        }
        std::vector<int> used(n, 0);
        std::vector<int> prev(n, -1);
        for (int i = 0; i < n; i++){
            if (is_cycle) break;
            if (!used[i]) dfs(i, g, used, prev);
        }
        if (!is_cycle){
            writeWord("NO\n");
        }
        else{
            writeWord("YES\n");
            int current = last;
            result.push_back(last + 1);
            while (current != answer){
                current = prev[current];
                result.push_back(current + 1);
            }
            for (int i = result.size() - 1; i >= 0; i--){
                writeInt(result[i], ' ');
            }
            writeChar('\n');
        }
        return 0;
    }
