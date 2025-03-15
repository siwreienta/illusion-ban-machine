#include "VF2.hpp"

namespace apotheosis {

// Проверка связей с тем, что уже утверждено
bool VF2::isFeasible(
    const std::vector<int> &M,
    int u,
    int v,
    Subgraph &subgraph
) {

    for (int u_predok : subgraph.m_reversed_edges[u]) {
        if (M[u_predok] != -1) {
            int v_predok = M[u_predok];
            if (!m_other_graph.m_edges[v_predok].contains(v)) {
                return false;
            }
        }
    }

    for (int u_dite : subgraph.m_edges[u]) {
        if (M[u_dite] != -1) {
            int v_dite = M[u_dite];
            if (!m_other_graph.m_edges[v].contains(v_dite)) {
                return false;
            }
        }
    }

    // Всё ок, мб еще чёт надо

    return true;
}

// Рекурсивная функция для поиска изоморфизма
bool VF2::VF2Recursive(std::vector<int> &M, Subgraph &subgraph, int M_counter) {
    if (subgraph.m_V == M_counter) {
        return true;
    }

    for (int u = 0; u < subgraph.m_V; ++u) {
        if (M[u] == -1) {  // Для этой вершины нет соответствия
            
            std::string type = subgraph.vertex_table[u];
            for (int v : m_other_graph.vertex_map[type]) {
                if (find(begin(M), end(M), v) == end(M) &&
                    isFeasible(M, u, v, subgraph)) {
                    M[u] = v;
                    if (VF2Recursive(M, subgraph, M_counter + 1)) {
                        return true;
                    }
                    
                    M[u] = -1;
                }
            }
        }
    }

    return false;
}

bool VF2::find_morph_subgraph(Subgraph &subgraph) {
    std::vector<int> M(subgraph.m_V, -1);  // v_subg -> v_otherg
    return VF2Recursive(M, subgraph, 0);
}

long double VF2::check() {
    int founded = 0;
    for (auto &subgraph : m_subgraphs) {
        if (find_morph_subgraph(subgraph)) {
            founded++;
        }
    }
    return static_cast<long double>(founded) / m_subgraphs.size();
}
}  // namespace apotheosis