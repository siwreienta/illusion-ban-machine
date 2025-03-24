#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

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

const int DEPTH_OF_DEVISION = 7;

class Subgraph;

class Graph {
public:
    int m_V;
    int m_E;
    std::string m_name;

    // Список смежности
    std::vector<std::set<int>> m_edges;

    std::unordered_map<std::string, std::vector<int>> vertex_map;
    std::vector<std::string> vertex_table;

    std::vector<int> bfs(int root);
    void make_subgraphs_and_put_into_vector(
        int root,
        std::vector<Subgraph> &subgraphs
    );

    Subgraph make_subgraph(std::vector<int> vertexes, int number);

public:
    friend class VF2;
    void add_vertex(std::string type);
    void add_edge(int v1, int v2);
    std::vector<Subgraph> devide_into_subgraphs();
    Graph(std::string name);
};

class Subgraph : public Graph {
private:
    std::vector<std::set<int>> m_reversed_edges;
    void dfs(int v, std::vector<int> &used);

public:
    friend class VF2;
    void add_vertex(std::string type);
    void add_edge(int v1, int v2);
    Subgraph(std::string name) : Graph(name){};
    friend bool check_svyaznost(Subgraph &subgraph);
};

}  // namespace apotheosis
#endif