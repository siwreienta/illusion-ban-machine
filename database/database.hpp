#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <pqxx/pqxx>
#include <stdexcept>
#include <string>

namespace apotheosis {

class database_errors : public std::exception {
private:
    std::string message;

public:
    database_errors(const std::string &msg) : message(msg) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class database {
private:
    pqxx::connection conn_;

public:
    explicit database() 
        : conn_("host=localhost dbname=apotheosis user=siwreienta password=mimimamomu") {
        if (!conn_.is_open()) {
            throw database_errors("База данных не открылась");
        }
    }

    ~database() = default;

    database(const database &) = delete;
    database &operator=(const database &) = delete;

    pqxx::connection &get_connection() {
        return conn_;
    }

    int add_user(const std::string &name, bool internal_use = false);
    int add_task(int contest_number, int task_number, bool internal_use = false);
    int load_code(
        const std::string &child_name,
        int task,
        const std::string &contest,
        const std::string &code
    );
    int load_graph(int solution_id, const std::string &graph);
    std::string get_graph(int graph_id);
    std::string get_solution(int solution_id);
    bool user_exists(const std::string &name);
    void delete_user(int user_id);
    pqxx::result get_user_solutions(const std::string &user_name);
};

}  // namespace apotheosis

#endif
