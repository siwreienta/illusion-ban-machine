#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

// #define APOTHEOSIS_DEBUG
#ifdef APOTHEOSIS_DEBUG
#include <chrono>
#endif

#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "errors.hpp"

namespace apotheosis {

long double check_two_graphs(const std::string &fpath_1, const std::string &fpath_2);
long double check_two_graphs_old(const std::string &fpath_1, const std::string &fpath_2);
std::vector<std::vector<long double>> check_all_in_dir(const std::string &dir_path);

constexpr int SUBGRAPH_SIZE = 7;

static inline uint64_t vector_hash(const std::vector<int>& nums) {
    uint64_t hash = 0;
    for (int num : nums) {
        uint64_t elem_hash = static_cast<uint64_t>(num);
        elem_hash = (elem_hash ^ (elem_hash << 16)) * 0x45d9f3b;
        hash ^= elem_hash;
    }
    return hash;
}

class Subgraph;

class Graph {
protected:
    int m_V;
    std::string m_name;

    // Список смежности
    std::vector<std::set<int>> m_edges;
    std::vector<int> m_dist;
    std::vector<std::vector<bool>> adjacency_matrix;

    std::unordered_map<std::string, std::vector<int>> vertex_map;
    std::vector<std::string> vertex_table;

    std::vector<std::vector<int>> m_subgraphs;

    std::set<int> m_roots;
    std::unordered_map<uint64_t,bool> est_li;

    Subgraph make_subgraph(const std::vector<int> &vertexes, int number);
    void make_subgraphs(int n, int last, std::vector<int> &taken_vertexes);
    std::vector<int> bfs(int root);
    void make_subgraphs_full(int root);

public:
    friend class VF2;
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    void devide_into_subgraphs();
    void start_read(int V);
    void end_read();
    void matrix_resize(int V);
    void devide_into_subgraphs_full();
    friend bool operator<(const Graph &l, const Graph &r){
    return l.m_name<r.m_name;
}
    Graph(std::string name);
};


class Subgraph : public Graph {
private:
    std::vector<std::set<int>> m_reversed_edges;
public:
    friend class VF2;
    void dfs(int v, std::vector<int> &used);
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    Subgraph(std::string name) : Graph(name){};
};

}  // namespace apotheosis
#endif