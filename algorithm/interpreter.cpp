#include "interpreter.hpp"

namespace apotheosis {

void Interpreter::load_graph() {
    // Формат команды:
    // "Получить *путь к графу*" - два токена.
    if (tokens.size() != 2) {
        throw invalid_command_format();
    }

    const std::string &filepath = tokens[1];
    std::ifstream is(filepath);
    if (!(is)) {
        throw unable_to_open(filepath);
    }

    std::string key;
    std::string name;
    if (!(is >> key >> name)) {
        throw not_a_grath(filepath);
    }
    if (key != "digraph") {
        throw not_a_grath(filepath);
    }
    Graph graph = GraphHandler::read_graph(name, std::move(is));
    std::cout << "Граф сохранён в ячейке номер "
              << gh.add_graph(std::move(graph)) << '\n';
}

Commands Interpreter::get_command() {
    std::string cmd = tokens[0];
    for (char &c : cmd) {
        c = tolower(c);
    }
    auto it = command_map.find(cmd);
    if (it != command_map.end()) {
        return it->second;
    }

    return BAD_COMMAND;
}

void Interpreter::run_command() {
    if (tokens.empty()) {
        throw invalid_command_format();
    }

    const auto command = get_command();
    switch (command) {
        case LOAD:
            load_graph();
            break;
        case BAD_COMMAND:
            throw unknown_command(tokens[0]);
    }
}

void Interpreter::start_interpreter() {
    std::string input;
    while (std::getline(std::cin, input)) {
        std::stringstream ss(input);
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        if ((tokens[0] == "exit" || tokens[0] == "выход")) {
            break;
        }
        try {
            run_command();
        } catch (std::exception &exception) {
            std::cout << exception.what() << '\n';
        }
    }
}

}  // namespace apotheosis
