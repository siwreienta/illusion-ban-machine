#include "apotheosis.hpp"

namespace apotheosis {

void print(std::vector<int> &vec) {
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void Graph::add_vertex(std::string type) {
    vertex_map[type].push_back(m_V);
    vertex_table.push_back(type);
    m_V++;
    m_edges.push_back({});
}

void Subgraph::add_vertex(std::string type) {
    Graph::add_vertex(type);
    m_reversed_edges.push_back({});
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

        if (dist[current] >= DEPTH_OF_DEVISION) {
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

Subgraph Graph::make_subgraph(std::vector<int> vertexes, int number) {
    Subgraph result(this->m_name + "_subgraph" + std::to_string(number));
    for (auto v : vertexes) {
        result.add_vertex(vertex_table[v]);
    }
    for (int i = 0; i < DEPTH_OF_DEVISION; i++) {
        for (int j = 0; j < DEPTH_OF_DEVISION; j++) {
            if (m_edges[vertexes[i]].contains(vertexes[j])) {
                result.add_edge(i, j);
            }
        }
    }
    return result;
}

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
    std::vector<int> used(DEPTH_OF_DEVISION, 0);
    subgraph.dfs(0, used);
    if (std::find(used.begin(), used.end(), 0) == used.end()) {
        return true;
    }
    return false;
}

void Graph::make_subgraphs_and_put_into_vector(
    int root,
    std::vector<Subgraph> &subgraphs
) {
    std::vector<int> close_vertexes = bfs(root);
    int size = close_vertexes.size();
    int depth = DEPTH_OF_DEVISION;

    // 7 форов, для DEPTH_OF_DIVISION = 7
    // Вполне естественно, что это надо поменять после MVP
    for (int i1 = 0; i1 < size - depth + 1; i1++) {
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
                                Subgraph maybe_subgraph =
                                    make_subgraph(vertexes, subgraphs.size());
                                if (check_svyaznost(maybe_subgraph)) {
                                    subgraphs.push_back(std::move(maybe_subgraph
                                    ));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<Subgraph> Graph::devide_into_subgraphs() {
    std::vector<Subgraph> subgraphs;
    for (int v = 0; v < m_V; v++) {
        make_subgraphs_and_put_into_vector(v, subgraphs);
    }
    return subgraphs;
}

void Graph::add_edge(int v1, int v2) {
    m_edges[v1].insert(v2);
}

void Subgraph::add_edge(int v1, int v2) {
    Graph::add_edge(v1, v2);
    m_reversed_edges[v2].insert(v1);
}

Graph::Graph(std::string name) : m_V(0), m_E(0), m_name(name) {
}

}  // namespace apotheosis