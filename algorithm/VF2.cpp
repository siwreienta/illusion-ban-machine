#include "VF2.hpp"

namespace apotheosis {

// Проверка связей с тем, что уже утверждено
bool VF2::isFeasible(
    const std::vector<int> &vertex_sootvetstvie,
    int u,
    int v,
    Subgraph &subgraph
) {
    for (int u_predok : subgraph.m_reversed_edges[u]) {
        if (vertex_sootvetstvie[u_predok] != -1) {
            int v_predok = vertex_sootvetstvie[u_predok];
            if (!m_other_graph.m_edges[v_predok].contains(v)) {
                return false;
            }
        }
    }

    for (int u_dite : subgraph.m_edges[u]) {
        if (vertex_sootvetstvie[u_dite] != -1) {
            int v_dite = vertex_sootvetstvie[u_dite];
            if (!m_other_graph.m_edges[v].contains(v_dite)) {
                return false;
            }
        }
    }

    // Всё ок, мб еще чёт надо

    return true;
}

// Рекурсивная функция для поиска изоморфизма
bool VF2::VF2Recursive(
    std::vector<int> &vertex_sootvetstvie,
    Subgraph &subgraph,
    int M_counter
) {
    if (subgraph.m_V == M_counter) {
        return true;
    }

    for (int u = 0; u < subgraph.m_V; ++u) {
        if (vertex_sootvetstvie[u] ==
            -1) {  // Для этой вершины нет соответствия

            std::string type = subgraph.vertex_table[u];
            for (int v : m_other_graph.vertex_map[type]) {
                if (find(
                        begin(vertex_sootvetstvie), end(vertex_sootvetstvie), v
                    ) == end(vertex_sootvetstvie) &&
                    isFeasible(vertex_sootvetstvie, u, v, subgraph)) {
                    vertex_sootvetstvie[u] = v;
                    if (VF2Recursive(
                            vertex_sootvetstvie, subgraph, M_counter + 1
                        )) {
                        return true;
                    }

                    vertex_sootvetstvie[u] = -1;
                }
            }
        }
    }

    return false;
}

bool VF2::find_morph_subgraph(Subgraph &subgraph) {
    std::vector<int> vertex_sootvetstvie(
        subgraph.m_V, -1
    );  // v_subg -> v_otherg
    return VF2Recursive(vertex_sootvetstvie, subgraph, 0);
}

long double VF2::check() {
    int founded = 0;
    for (auto &subgraph : m_subgraphs) {
        if (find_morph_subgraph(subgraph)) {
            founded++;
        }
    }
    std::cout << founded << " " << m_subgraphs.size() << std::endl;
    if (m_subgraphs.size() == 0) {
        return -1;
    }
    return static_cast<long double>(founded) / m_subgraphs.size();
}
}  // namespace apotheosis