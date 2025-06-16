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
    Graph &m_this_graph;
    Graph &m_other_graph;
    std::vector<int> vertex_map;

    bool VF2Recursive(
        std::vector<int> &vertex_sootvetstvie,
        const Subgraph &subgraph,
        int M_counter
    );
    bool check_connects(
        const std::vector<int> &vertex_sootvetstvie,
        int u,
        int v,
        const Subgraph &subgraph
    );

public:
    VF2(Graph &this_graph, Graph &other_graph)
        : m_this_graph(this_graph),
          m_other_graph(other_graph),
          vertex_map(std::vector<int>(SUBGRAPH_SIZE, -1)){};

    long double check();
};

}  // namespace apotheosis
#endif