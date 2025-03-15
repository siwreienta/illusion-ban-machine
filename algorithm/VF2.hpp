#ifndef VF2_HPP_
#define VF2_HPP_
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "apotheosis.hpp"
#include "errors.hpp"

namespace apotheosis {

class VF2 {
private:
    std::vector<Subgraph> m_subgraphs;
    Graph m_other_graph;

    bool find_morph_subgraph(Subgraph &subgraph); 
    bool VF2Recursive(
        std::vector<int> &M,
        Subgraph &subgraph,
        int M_counter
    );
    bool isFeasible(
        const std::vector<int> &M,
        int u,
        int v,
        Subgraph &subgraph
    );

public:
    VF2(std::vector<Subgraph> &subgraphs, Graph &other_graph)
        : m_subgraphs(subgraphs), m_other_graph(other_graph){};
    
    void dfs();
    long double check();

};

}  // namespace apotheosis
#endif