#include "apotheosis.hpp"

namespace apotheosis {

void Subgraph::dfs(int v, std::vector<int> &used) {
    if (used[v]) {
        return;
    }
    used[v] = 1;
    for (auto u : m_edges[v]) {
        dfs(u, used);
    }
    for (auto u : m_reversed_edges[v]) {
        dfs(u, used);
    }
}

bool check_svyaznost(Subgraph &subgraph) {
    std::vector<int> used(SUBGRAPH_SIZE, 0);
    subgraph.dfs(0, used);
    if (std::find(used.begin(), used.end(), 0) == used.end()) {
        return true;
    }
    return false;
}

std::vector<int> Graph::bfs(int root) {
    std::vector<int> result;
    std::queue<int> queue;
    std::vector<int> dist(m_V, 100);

    dist[root] = 0;

    queue.push(root);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (dist[current] >= SUBGRAPH_SIZE) {
            break;
        }

        result.push_back(current);

        for (int neighbor : m_edges[current]) {
            if (dist[neighbor] == 100 && m_dist[neighbor]) {
                dist[neighbor] = dist[current] + 1;
                queue.push(neighbor);
            }
        }
    }
    return result;
}

void Graph::make_subgraphs_full(int root) {
    std::vector<int> close_vertexes = bfs(root);
    int size = close_vertexes.size();
    int depth = SUBGRAPH_SIZE;
    int i1 = 0;
    for (int i2 = i1 + 1; i2 < size - depth + 2; i2++) {
        for (int i3 = i2 + 1; i3 < size - depth + 3; i3++) {
            for (int i4 = i3 + 1; i4 < size - depth + 4; i4++) {
                for (int i5 = i4 + 1; i5 < size - depth + 5; i5++) {
                    for (int i6 = i5 + 1; i6 < size - depth + 6; i6++) {
                        for (int i7 = i6 + 1; i7 < size - depth + 7; i7++) {
                            std::vector<int> vertexes = {
                                close_vertexes[i1], close_vertexes[i2],
                                close_vertexes[i3], close_vertexes[i4],
                                close_vertexes[i5], close_vertexes[i6],
                                close_vertexes[i7]};
                            auto hash =vector_hash(vertexes);
                            if (!est_li.contains(hash)){
                                est_li[hash] = true;
                                Subgraph maybe_subgraph =
                                    make_subgraph(vertexes, m_subgraphs.size());
                                if (check_svyaznost(maybe_subgraph)) {
                                    m_subgraphs.push_back(vertexes);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Graph::devide_into_subgraphs_full() {
#ifdef APOTHEOSIS_DEBUG
    std::cout << this->m_name << "'s devide_into_subgraphs_full()\n ";
#endif
    for (int root = 0; root < m_V; root++) {
        if (m_dist[root]) {
            make_subgraphs_full(root);
        }
    }
}

}  // namespace apotheosis