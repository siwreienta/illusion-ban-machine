#include "VF2.hpp"

namespace apotheosis {

bool VF2::check_connects(
    const std::vector<int> &vertex_map,
    int u,
    int v,
    Subgraph &subgraph
) {
    for (int u_dite : subgraph.m_edges[u]) {
        if (vertex_map[u_dite] != -1) {
            int v_dite = vertex_map[u_dite];
            if (!m_other_graph.adjacency_matrix[v][v_dite]) {
                return false;
            }
        }
    }

    for (int u_predok : subgraph.m_reversed_edges[u]) {
        if (vertex_map[u_predok] != -1) {
            int v_predok = vertex_map[u_predok];
            if (!m_other_graph.adjacency_matrix[v_predok][v]) {
                return false;
            }
        }
    }

    return true;
}

bool VF2::VF2Recursive(
    std::vector<int> &vertex_map,
    Subgraph &subgraph,
    int u
) {
    if (subgraph.m_V == u) {
        return true;
    }

    std::string type = subgraph.vertex_table[u];
    for (int v : m_other_graph.vertex_map[type]) {
        if (find(begin(vertex_map), end(vertex_map), v) == end(vertex_map) &&
            check_connects(vertex_map, u, v, subgraph)) {
            vertex_map[u] = v;
            if (VF2Recursive(vertex_map, subgraph, u + 1)) {
                return true;
            }
        }
    }

    return false;
}

long double VF2::check() {
    int founded = 0;
    int i = 0;
    for (auto &vertexes : m_this_graph.m_subgraphs) {
        Subgraph subgraph = m_this_graph.make_subgraph(vertexes, i++);
        vertex_map.assign(DEPTH_OF_DEVISION, -1);

        if (VF2Recursive(vertex_map, subgraph, 0)) {
            founded++;
        }
    }
    if (m_this_graph.m_subgraphs.size() == 0) {
        return -1;
    }
    return static_cast<long double>(founded) / m_this_graph.m_subgraphs.size();
}
}  // namespace apotheosis