#include "apotheosis.hpp"

namespace apotheosis {

void Graph::add_vertex(std::string &type) {
    vertex_map[type].push_back(m_V);
    m_roots.insert(m_V);
    vertex_table.push_back(type);
    m_V++;
    m_edges.push_back({});
}

void Subgraph::add_vertex(std::string &type) {
    vertex_map[type].push_back(m_V);
    m_roots.insert(m_V);
    vertex_table.push_back(type);
    m_V++;
    m_edges.push_back({});
    m_reversed_edges.push_back({});
}

void Graph::add_edge(int v1, int v2) {
    m_edges[v1].insert(v2);
    m_roots.erase(v2);
}

void Subgraph::add_edge(int v1, int v2){
    m_edges[v1].insert(v2);
    m_reversed_edges[v2].insert(v1);
    m_roots.erase(v2);
}


Graph::Graph(std::string name) : m_V(0), m_E(0), m_name(name) {
}

}  // namespace apotheosis