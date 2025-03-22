#ifndef GRAPHHANDLER_HPP_
#define GRAPHHANDLER_HPP_
#include "apotheosis.hpp"
#include "errors.hpp"

namespace apotheosis {

class GraphHandler {
private:
    std::vector<Graph> graphs;

public:
    void privet();
    static Graph read_graph(std::string &name, std::ifstream is);
    int add_graph(Graph graph);
};

}  // namespace apotheosis
#endif