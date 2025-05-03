#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

#define APOTHEOSIS_DEBAG
#ifdef APOTHEOSIS_DEBAG
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

long double check_two_graphs(std::string &fpath_1, std::string &fpath_2);

const int DEPTH_OF_DEVISION = 7;

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

    Subgraph make_subgraph(const std::vector<int> &vertexes, int number);
    void make_subgraphs(int n, int last, std::vector<int> &taken_vertexes);

public:
    friend class VF2;
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    void devide_into_subgraphs();
    void start_read(int V);
    void end_read();
    void matrix_resize(int V);
    Graph(std::string name);
};

class Subgraph : public Graph {
private:
    std::vector<std::set<int>> m_reversed_edges;

public:
    friend class VF2;
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    Subgraph(std::string name) : Graph(name){};
};

}  // namespace apotheosis
#endif