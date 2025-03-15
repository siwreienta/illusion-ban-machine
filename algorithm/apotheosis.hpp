#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "errors.hpp"

namespace apotheosis {

class Subgraph;

class Graph {
public:
    int m_V;
    int m_E;
    std::string m_name;
    std::vector<std::set<int>> m_edges;
    std::unordered_map<std::string, std::vector<int>> vertex_map;
    std::vector<std::string> vertex_table;
    std::set<int>
        m_roots;  // Для создания подграфов (subgraph - дурацкое слово)

public:
    friend class VF2;
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    std::vector<Subgraph> make_subgraphs();
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