#include "apotheosis.hpp"

namespace apotheosis {


void Graph::add_vertex(std::string type) {
    vertex_map[type].push_back(m_V);
    vertex_table.push_back(type);
    m_roots.insert(m_V);
    m_V++;
    m_edges.push_back({});
}

void Subgraph::add_vertex(std::string type) {
    Graph::add_vertex(type);
    m_reversed_edges.push_back({});
}

void Graph::add_edge(int v1, int v2) {
    if (v1 != v2) {
        m_edges[v1].insert(v2);
        m_roots.erase(v2);
    }
}

void Subgraph::add_edge(int v1, int v2) {
    Graph::add_edge(v1, v2);
    m_reversed_edges[v2].insert(v1);
}

void Graph::end_read() {
    std::queue<int> queue;
    m_dist.resize(m_V, 10000);
    for (auto root : m_roots) {
        m_dist[root] = 0;
        queue.push(root);
    }

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neighbor : m_edges[current]) {
            if (m_dist[neighbor] > m_dist[current] + 1) {
                m_dist[neighbor] = m_dist[current] + 1;
                queue.push(neighbor);
            }
        }
    }
    m_roots.clear();
}

Subgraph Graph::make_subgraph(const std::vector<int> vertexes, int number) {
    Subgraph result(this->m_name + "_subgraph_" + std::to_string(number));
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

void Graph::make_subgraphs(int n, int last, std::vector<int> &taken_vertexes) {
    int v = taken_vertexes[n];
    int i = 0;
    for (auto u : m_edges[v]) {
        for (int j = 0; j < n; j++) {
            if (m_edges[taken_vertexes[j]].contains(u)) {
                goto end_of_iteration;
            }
        }
        if (i > last && m_dist[u] > m_dist[v]) {
            taken_vertexes.push_back(u);
            if (taken_vertexes.size() == DEPTH_OF_DEVISION) {
                m_subgraphs.push_back(
                    make_subgraph(taken_vertexes, m_subgraphs.size())
                );
            } else {
                make_subgraphs(n, i, taken_vertexes);
                make_subgraphs(n + 1, -1, taken_vertexes);
            }
            taken_vertexes.pop_back();
        }      
    end_of_iteration:
        i++;
    }
}

std::vector<Subgraph> &Graph::get_subgraphs() {
    if (m_subgraphs.empty()) {
        m_subgraphs.reserve(15000);
        std::vector<int> taken_vertexes;
        taken_vertexes.reserve(DEPTH_OF_DEVISION + 1);
        for (int root = 0; root < m_V; root++) {
            if (m_dist[root]) {
                taken_vertexes.assign(1, root);
                make_subgraphs(0, -1, taken_vertexes);
            }
        }
    }
    std::cout << "m_subgraphs.size() = " << m_subgraphs.size() << '\n';
    return m_subgraphs;
}

Graph::Graph(std::string name) : m_V(0), m_E(0), m_name(name) {
}

}  // namespace apotheosis