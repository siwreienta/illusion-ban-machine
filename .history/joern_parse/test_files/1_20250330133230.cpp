#define FAST_ALLOCATOR_MEMORY 1e8
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include "optimization.h"

using namespace std;

int n, m, e, v, start_cycle = -1;
vector <int> cycle;
int dfs(int v, vector<int> &visited, vector<vector<int>> &vec){
    visited[v] = 1;
    for (auto i: vec[v]){
        if (visited[i] == 0){
            cycle[i] = v;
            int u = dfs(i, visited, vec);
            if (u == 1) return u;
        }
        else if(visited [i] == 1){
            cycle[i] = v;
            start_cycle = i;
            return 1;
        }
    }
    visited[v] = 2;
    return 0;
}

int main() {
    n = readInt();
    m = readInt();
    vector<vector<int>> vec(n);
    vector <int> visited(n, 0);
    cycle.resize(n, -1);
    for (int i = 0; i < m; i++) {
        e = readInt() - 1;
        v = readInt() - 1;
        vec[e].push_back(v);
    }
    for (int i = 0; i < n; i++) {
        if(visited[i] == 0){
            if (dfs(i, visited, vec) == 1){
                break;
            }
        }
    }
    if(start_cycle == -1){
        writeWord("NO\n");
    }
    else{
        vector <int> otv;
        e = start_cycle;
        writeWord("YES\n");
        while (true){
            otv.push_back(start_cycle + 1);
            start_cycle = cycle[start_cycle];
            if(e == start_cycle){
                otv.push_back(e + 1);
                break;
            }
        }
        for (int i = otv.size() - 1; i > 0; i--) writeInt(otv[i], ' ');
    }
    return 0;
}
