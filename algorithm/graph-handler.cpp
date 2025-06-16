#include "graph-handler.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace apotheosis {

void GraphHandler::sort() {
    std::sort(graphs.begin(), graphs.end());
}

std::vector<std::vector<long double>> check_all_in_dir(const std::string &dir_path) {
    GraphHandler gh;
    for (const auto &dir_entry :
         std::filesystem::directory_iterator(dir_path)) {
        if (dir_entry.is_regular_file() &&
            dir_entry.path().extension() == ".dot") {
            gh.read_graph(dir_entry.path());
        }
    }
    gh.sort();
    std::vector<std::vector<long double>> res(gh.size(), std::vector<long double>(gh.size(), 0));
    for (int i = 0; i < gh.size(); i++) {
        for (int j = i + 1; j < gh.size(); j++) {
            res[i][j] = gh.check(i, j);
        }
    }
    return res;
}

long double
check_two_graphs(const std::string &fpath_1, const std::string &fpath_2) {
    GraphHandler gh;
    gh.read_graph(fpath_1);
    gh.read_graph(fpath_2);
    return gh.check(0, 1);
}

long double
check_two_graphs_old(const std::string &fpath_1, const std::string &fpath_2) {
    GraphHandler gh;
    gh.read_graph(fpath_1);
    gh.read_graph(fpath_2);
    return gh.check_full(0, 1);
}

long double GraphHandler::check_full(int first_number, int second_number) {
    graphs[first_number].devide_into_subgraphs_full();
    graphs[second_number].devide_into_subgraphs_full();
    long double res_1 =
        VF2(graphs[first_number], graphs[second_number]).check();
    long double res_2 =
        VF2(graphs[second_number], graphs[first_number]).check();
    return (res_1 + res_2) / 2;
}

long double GraphHandler::check(int first_number, int second_number) {
#ifdef APOTHEOSIS_DEBUG
    std::cout << "check(" << first_number << ", " << second_number << ")\n\n";
    auto start = std::chrono::steady_clock::now();
#endif
    graphs[first_number].devide_into_subgraphs();
    graphs[second_number].devide_into_subgraphs();
    long double res_1 =
        VF2(graphs[first_number], graphs[second_number]).check();
#ifdef APOTHEOSIS_DEBUG
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Первая проверка заняла " << elapsed.count() << " секунд\n";
    start = std::chrono::steady_clock::now();
#endif
    long double res_2 =
        VF2(graphs[second_number], graphs[first_number]).check();
#ifdef APOTHEOSIS_DEBUG
    end = std::chrono::steady_clock::now();
    elapsed = end - start;
    std::cout << "Вторая проверка заняла " << elapsed.count() << " секунд\n";
#endif
    return (res_1 + res_2) / 2;
}

void GraphHandler::read_graph(std::string filepath) {
#ifdef APOTHEOSIS_DEBUG
    std::cout << "reading graph " << filepath << '\n';
#endif
    std::ifstream is(filepath);
    if (!(is)) {
        throw unable_to_open(filepath);
    }

    std::string key;
    std::string name;
    if (!(is >> key >> name) || key != "digraph") {
        throw not_a_graph(filepath);
    }

    Graph graph(name);

    int V = 0;  // Количество вершин
    int E = 0;  // Количество ребер
    if (!(is >> V >> E)) {
        throw bad_read();
    }
    graph.matrix_resize(V);

    // Чтение вершин
    std::string vertex_name;
    std::string vertex_type;

    for (int i = 0; i < V; i++) {
        if (!(is >> vertex_name >> vertex_type)) {
            throw bad_read();
        }
        graph.add_vertex(vertex_type);
    }

    // В дальнейшем должны быть инты, дабы не костылить
    int v1 = 0;
    int v2 = 0;
    for (int i = 0; i < E; i++) {
        if (!(is >> v1 >> v2)) {
            throw bad_read();
        }
        graph.add_edge(v1, v2);
    }
    graph.end_read();
    graphs.push_back(std::move(graph));
}

}  // namespace apotheosis