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
    // int v=0;
    // is>>v;
    // std::cout<<v<<std::endl;
    Graph graph = GraphHandler::read_graph(name, is);
    gh.add_graph(std::move(graph));
    if (!is_manual) {
        std::cout << "Граф сохранён в ячейке номер " << gh.size() - 1 << '\n';
    }
}

bool is_uint(std::string &s) {
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

void Interpreter::check() {
    // Формат команды:
    // "Чек *номер графа* *номер графа*" - три токена.
    if (tokens.size() != 3) {
        throw invalid_command_format();
    }
    int first_number = 0;
    int second_number = 1;
    if (is_uint(tokens[1])) {
        first_number = std::stoi(tokens[1]);
    } else {
        throw invalid_command_format();
    }
    if (is_uint(tokens[2])) {
        second_number = std::stoi(tokens[2]);
    } else {
        throw invalid_command_format();
    }

    if (first_number > second_number) {
        std::swap(first_number, second_number);
    }

    if (second_number >= gh.size()) {
        throw too_big_number(second_number, gh.size());
    }

    long double result = gh.check(first_number, second_number);

    if (is_manual) {
        check_result = result;
    } else {
        std::cout << "Похожесть графов: " << result << '\n';
    }
}

Commands Interpreter::get_command() {
    std::string cmd = tolower_ru(tokens[0]);

    if (command_map.contains(cmd)) {
        return command_map.at(cmd);
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
        case PRIVET:
            gh.privet();
            break;
        case CHECK:
            check();
            break;
        case BAD_COMMAND:
            throw unknown_command(tokens[0]);
    }
}

void Interpreter::start_interpreter() {
    is_manual = false;
    std::string input;
    while (std::getline(std::cin, input)) {
        tokens.resize(0);
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

long double check_two_graphs(std::string path_1, std::string path_2) {
    Interpreter interpreter;
    std::stringstream ss;
    ss << "load " << path_1 << " endcmd load " << path_2
       << " endcmd check 0 1 endcmd";
    interpreter.start_manual_work(std::move(ss));
    return interpreter.return_check_result();
}

void Interpreter::start_manual_work(std::stringstream ss) {
    is_manual = true;
    std::string token;
    while (ss >> token) {
        if (token == "endcmd") {
            try {
                run_command();
            } catch (std::exception &exception) {
                std::cout << exception.what() << '\n';
            }
            tokens.resize(0);
        } else {
            tokens.push_back(std::move(token));
        }
    }

    is_manual = false;
}

}  // namespace apotheosis
