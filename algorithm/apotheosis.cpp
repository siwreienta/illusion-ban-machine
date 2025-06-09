#include "apotheosis.hpp"

namespace apotheosis {


void Graph::add_vertex(std::string &type) {
    vertex_map[type].push_back(m_V);
    vertex_table.push_back(type);
    m_roots.insert(m_V);
    m_V++;
    m_edges.push_back({});
}

void Subgraph::add_vertex(std::string &type) {
    Graph::add_vertex(type);
    m_reversed_edges.push_back({});
}

void Graph::add_edge(int v1, int v2) {
    if (v1 != v2) {
        m_edges[v1].insert(v2);
        m_roots.erase(v2);
        adjacency_matrix[v1][v2] = true;
    }
}

void Subgraph::add_edge(int v1, int v2) {
    m_edges[v1].insert(v2);
    m_reversed_edges[v2].insert(v1);
}

void Graph::matrix_resize(int V) {
    adjacency_matrix.resize(V, std::vector<bool>(V, false));
}

void Graph::end_read() {
#ifdef APOTHEOSIS_DEBUG
    std::cout << this->m_name << "'s end_read()\n\n";
#endif
    std::queue<int> queue;
    m_dist.resize(m_V, 10000);
    std::vector<int> used(m_V,0);
    for (auto root : m_roots) {
        m_dist[root] = 0;
        used[root] = 1;
        queue.push(root);
    }

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        used[current] = 0;

        for (int neighbor : m_edges[current]) {
            if (m_dist[neighbor] > m_dist[current] + 1) {
                m_dist[neighbor] = m_dist[current] + 1;
                if(used[neighbor]==0)
                {queue.push(neighbor);
                used[neighbor]=1;}
            }
        }
    }
    m_roots.clear();
}

Subgraph Graph::make_subgraph(const std::vector<int> &vertexes, int number) {
    Subgraph result(this->m_name + "_subgraph_" + std::to_string(number));
    for (auto v : vertexes) {
        result.add_vertex(vertex_table[v]);
    }
    for (int i = 0; i < SUBGRAPH_SIZE; i++) {
        for (int j = 0; j < SUBGRAPH_SIZE; j++) {
            if (adjacency_matrix[vertexes[i]][vertexes[j]]) {
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
        if (i > last && m_dist[u] >= m_dist[v]) {
            taken_vertexes.push_back(u);
            if (taken_vertexes.size() == SUBGRAPH_SIZE) {
                auto hash =vector_hash(taken_vertexes);
                if (!est_li.contains(hash)){
                    est_li[hash] = true;
                m_subgraphs.push_back(taken_vertexes);}
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

void Graph::devide_into_subgraphs() {
    if (m_subgraphs.empty()) {
#ifdef APOTHEOSIS_DEBUG
        std::cout << this->m_name << "'s devide_into_subgraphs()\n";
        auto start = std::chrono::steady_clock::now();
#endif

        std::vector<int> taken_vertexes;
        taken_vertexes.reserve(SUBGRAPH_SIZE + 1);
        for (int root = 0; root < m_V; root++) {
            if (m_dist[root]) {
                taken_vertexes.assign(1, root);
                make_subgraphs(0, -1, taken_vertexes);
            }
        }

        if (m_subgraphs.size() < 100) {
            devide_into_subgraphs_full();
        }
#ifdef APOTHEOSIS_DEBUG
        std::cout << this->m_name
                  << "'s m_subgraphs.size() = " << m_subgraphs.size() << '\n';
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Деление заняло " << elapsed.count() << " секунд\n\n";
#endif
    }
    est_li.clear();
}

Graph::Graph(std::string name) : m_V(0), m_name(name) {
}

}  // namespace apotheosis