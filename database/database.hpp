#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <pqxx/pqxx>
#include <string>
#include <stdexcept>

namespace apotheosis {

class database_errors : public std::exception {
private:
    std::string message;

public:
    database_errors(const std::string &msg) : message(msg) {
    }

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class database {
private:
    pqxx::connection conn_;

public:
    explicit database() 
    : conn_("host=localhost dbname=apotheosis user=sofa password=w8uSZJZo") {
    if (!conn_.is_open()) {
        throw database_errors("База данных не открылась");
    }
}

    int add_user(const std::string &name, bool internal_use = false);
    int add_task(int contest_number, int task_number, bool internal_use = false);
    int load_code(
        const std::string &child_name,
        int task,
        int contest,
        const std::string &code
    );
    int load_graph(int solution_id, const std::string &graph);
    const std::string get_graph(int graph_id);
    const std::string get_solution(int solution_id);

    ~database() = default;
};

}  // namespace apotheosis
#endif
