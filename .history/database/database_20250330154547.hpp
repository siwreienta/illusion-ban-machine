#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <pqxx/pqxx>

namespace apotheosis {

class database{
    public:
    void connect();
    void load_code(const std::string& child_name, int task, const std::string& code);
    void load_graph(const std::string& child_name, int task, const std::string& graph);
    const std::string return_dot_file(int graph_id);
    void return_raw_code(int code_id);
};

}  // namespace apotheosis
#endif
