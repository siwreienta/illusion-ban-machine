#include "graph-handler.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace apotheosis {

void GraphHandler::privet() {
    std::cout << "Привет, сокомандники!\nZZZZZ\n";
}

void GraphHandler::add_graph(Graph graph) {
    graphs.push_back(graph);
}

long double GraphHandler::check(int first_number, int second_number) {
    std::vector<Subgraph> subgraphs =
        graphs[first_number].devide_into_subgraphs();
    VF2 vf2(subgraphs, graphs[second_number]);
    return vf2.check();
}

Graph GraphHandler::read_graph(std::string &name, std::ifstream &is) {
    Graph graph(name);

    // Подразумеваю, что вершины будут хорошо переименованы, пока нет нужна мапа
    std::unordered_map<std::string, int> vertex_map;

    int V = 0;  // Количество вершин
    int E = 0;  // Количество ребер
    if (!(is >> V >> E)) {
        throw bad_read();
    }

    // Чтение вершин
    std::string vertex_name;
    std::string vertex_type;

    for (int i = 0; i < V; i++) {
        if (!(is >> vertex_name >> vertex_type)) {
            throw bad_read();
        }
        vertex_map[vertex_name] = i;
        graph.add_vertex(vertex_type);
    }

    // В дальнейшем должны быть инты, дабы не костылить
    std::string vertex_1_name;
    std::string vertex_2_name;
    for (int i = 0; i < E; i++) {
        if (!(is >> vertex_1_name >> vertex_2_name)) {
            throw bad_read();
        }
        int v1 = vertex_map[vertex_1_name];
        int v2 = vertex_map[vertex_2_name];
        graph.add_edge(v1, v2);
    }
    return graph;
}

}  // namespace apotheosis