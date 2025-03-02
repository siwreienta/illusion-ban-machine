#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_
#include "apotheosis.hpp"
#include "errors.hpp"
#include "graph-handler.hpp"

namespace apotheosis {

enum class Commands { LOAD, BAD_COMMAND };
using enum Commands;

static const std::map<std::string, Commands> command_map = {
    {"получить", LOAD},
    {"загрузить", LOAD},
    {"load", LOAD}};

class Interpreter {
private:
    GraphHandler gh;
    std::vector<std::string> tokens;

    void load_graph();

    Commands get_command();
    void run_command();

public:
    void start_interpreter();
};
}  // namespace apotheosis
#endif