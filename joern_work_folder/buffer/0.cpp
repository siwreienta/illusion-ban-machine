#include "optimization.h"
#include <vector>
#include <string>

std::vector<int> a;
std::vector<std::vector<int>> g;
std::vector<int> result;
int cur_size = 0;

void dfs(int v, std::vector<std::pair<int, int>> &set, std::vector<std::vector<std::pair<int, int>>> &get) {
    for (auto u : g[v]) {
        std::swap(a[set[u].first], set[u].second);
        dfs(u, set, get);
        std::swap(a[set[u].first], set[u].second);
    }
    for (auto x : get[v]) result[cur_size + x.second] = a[x.first];
}

int main() {
    int n, m, idx = 0, k = 1;
    while (!seekEof()) {
        n = readInt();
        a.resize(n);
        for (int i = 0; i < n; i++) a[i] = readInt();
        m = readInt();
        g.resize(m);
        std::vector<std::pair<int, int>> set(1);
        std::vector<std::vector<std::pair<int, int>>> get(m);
        char c[20];
        std::string s;
        while (m--) {
            readWord(c);
            s = c;
            if (s == "create") {
                g[readInt() - 1].push_back(k++);
                set.push_back({readInt() - 1, readInt()});
            } 
            else get[readInt() - 1].push_back({readInt() - 1, idx++});
        }
        result.resize(cur_size + idx);
        dfs(0, set, get);
        g.clear();
        idx = 0;
        k = 1;
        cur_size = result.size();
    }
    for (auto x : result) writeInt(x, '\n');
    return 0;
}
