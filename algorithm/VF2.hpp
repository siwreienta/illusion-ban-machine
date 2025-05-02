#ifndef VF2_HPP_
#define VF2_HPP_
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "apotheosis.hpp"
#include "errors.hpp"

namespace apotheosis {

class VF2 {
private:
    std::vector<Subgraph> m_subgraphs;
    Graph m_other_graph;
    std::vector<int> vertex_map;

    bool VF2Recursive(
        std::vector<int> &vertex_sootvetstvie,
        Subgraph &subgraph,
        int M_counter
    );
    bool check_connects(
        const std::vector<int> &vertex_sootvetstvie,
        int u,
        int v,
        Subgraph &subgraph
    );

public:
    VF2(std::vector<Subgraph> &subgraphs, Graph &other_graph)
        : m_subgraphs(subgraphs), m_other_graph(other_graph), vertex_map(std::vector<int> (DEPTH_OF_DEVISION, -1)){};

    void dfs();
    long double check();
};

}  // namespace apotheosis
#endif