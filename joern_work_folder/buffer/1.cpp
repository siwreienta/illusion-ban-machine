
// Created 29.04.2025 by Melnikov Kirill

#include <vector>
#include "optimization.h"

std::vector<int> array;

struct CreateQuery {
    int el_index, val;
};

struct GetQuery {
    int el_index, get_index;
};

std::vector<std::vector<int>> versions_dfs_tree;
std::vector<CreateQuery> crt_queries;
std::vector<std::vector<GetQuery>> get_queries;
std::vector<int> answer;

void dfs(int ver) {
    for (auto [el_index, get_index] : get_queries[ver]) {
        answer[get_index] = array[el_index];
    }

    for (auto to_ver : versions_dfs_tree[ver]) {
        auto [el_index, val] = crt_queries[to_ver];

        int old_val = array[el_index];
        array[el_index] = val;
        dfs(to_ver);
        array[el_index] = old_val;
    }
}

int main() {
    while (!seekEof()) {
        int n = readInt();
        array.resize(n);
        for (int i = 0; i < n; ++i) {
            array[i] = readInt();
        }

        int m = readInt();

        int version_count = 1;
        int get_count = 0;      // Для верного порядка вывода ответа
        versions_dfs_tree.emplace_back();
        crt_queries.emplace_back();
        get_queries.emplace_back();

        for (int i = 0; i < m; ++i) {
            char buff[8];
            readWord(buff);
            
            if (buff[0] == 'c') {
                int version = readInt();
                int el_index = readInt();
                int val = readInt();
                --version;
                --el_index;
                
                versions_dfs_tree[version].push_back(version_count);
                ++version_count;

                versions_dfs_tree.emplace_back();
                crt_queries.push_back({el_index, val});
                get_queries.emplace_back();
            } else {
                int version = readInt();
                int el_index = readInt();
                --version;
                --el_index;

                get_queries[version].push_back({el_index, get_count});
                ++get_count;
            }
        }

        answer.resize(get_count);
        dfs(0);

        for (auto it : answer) {
            writeInt(it, '\n');
        }

        versions_dfs_tree.clear();
        crt_queries.clear();
        get_queries.clear();
    }
}