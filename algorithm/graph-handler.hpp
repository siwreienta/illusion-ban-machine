#ifndef GRAPHHANDLER_HPP_
#define GRAPHHANDLER_HPP_
#include "VF2.hpp"
#include "apotheosis.hpp"
#include "errors.hpp"

namespace apotheosis {

class GraphHandler {
private:
    std::vector<Graph> graphs;

public:
    void read_graph(std::string filepath);

    int size() {
        return static_cast<int>(graphs.size());
    }

    long double check(int first_number, int second_number);
};

}  // namespace apotheosis
#endif