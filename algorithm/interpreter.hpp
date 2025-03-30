#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_
#include "apotheosis.hpp"
#include "errors.hpp"
#include "graph-handler.hpp"

namespace apotheosis {

enum class Commands { LOAD, PRIVET, CHECK, BAD_COMMAND };
using enum Commands;

static const std::unordered_map<std::string, Commands> command_map = {
    {"получить", LOAD},  {"загрузить", LOAD}, {"привет", PRIVET},
    {"привет!", PRIVET}, {"чек", CHECK},      {"проверить", CHECK},
    {"check", CHECK},    {"load", LOAD}};

long double check_two_graphs(std::string path_1, std::string path_2);

class Interpreter {
private:
    GraphHandler gh;
    std::vector<std::string> tokens;
    bool is_manual = false;
    long double check_result = 0;

    void load_graph();
    void check();

    Commands get_command();
    void run_command();

public:
    void start_interpreter();
    void start_manual_work(std::stringstream ss);

    long double return_check_result() {
        return check_result;
    }
};
}  // namespace apotheosis
#endif