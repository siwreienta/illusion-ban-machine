#include "apotheosis.hpp"

namespace apotheosis {

void Graph::add_vertex(std::string &type) {
    vertex_map[type].push_back(m_V);
    vertex_table.push_back(std::move(type));
    m_V++;
}

void Graph::add_edge(int v1, int v2) {
    m_edges[v1].push_back(v2);
}

Graph::Graph(std::string name) : m_V(0), m_E(0), m_name(name) {
}

}  // namespace apotheosis