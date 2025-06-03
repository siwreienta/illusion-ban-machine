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
            if (dist[neighbor] == 100) {
                dist[neighbor] = dist[current] + 1;
                queue.push(neighbor);
            }
        }
    }
    return result;
}

void Graph::make_subgraphs_and_put_into_vector(
    int root,
    std::set<std::vector<int>> &est_li
) {
    std::vector<int> close_vertexes = bfs(root);
    int size = close_vertexes.size();
    int depth = SUBGRAPH_SIZE;

    // 5 форов, для DEPTH_OF_DIVISION = 5
    // Вполне естественно, что это надо поменять после MVP
    int i1 = 0;
    for (int i2 = i1 + 1; i2 < size - depth + 2; i2++) {
        for (int i3 = i2 + 1; i3 < size - depth + 3; i3++) {
            for (int i4 = i3 + 1; i4 < size - depth + 4; i4++) {
                for (int i5 = i4 + 1; i5 < size - depth + 5; i5++) {
                    std::vector<int> vertexes = {
                        close_vertexes[i1], close_vertexes[i2],
                        close_vertexes[i3], close_vertexes[i4],
                        close_vertexes[i5]};
                    sort(vertexes.begin(), vertexes.end());
                    if (!est_li.contains(vertexes)) {
                        est_li.insert(vertexes);
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

void Graph::make_subgraphs_old() {
    if (m_subgraphs.empty()) {
        std::set<std::vector<int>> est_li;
        for (int root = 0; root < m_V; root++) {
            if (m_dist[root]) {
                make_subgraphs_and_put_into_vector(root, est_li);
            }
        }
    }
}


}  // namespace apotheosis