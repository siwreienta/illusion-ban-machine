#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "errors.hpp"

namespace apotheosis {

class Graph {
private:
    [[maybe_unused]] int m_V;
    [[maybe_unused]] int m_E;
    std::string m_name;
    std::vector<std::vector<int>> m_edges;
    std::unordered_map<std::string, std::vector<int>>
        vertex_map;  // Нужен будет для алгоритма
    std::vector<std::string> vertex_table;

public:
    void add_vertex(std::string &type);
    void add_edge(int v1, int v2);
    Graph(std::string name);
};

}  // namespace apotheosis
#endif