#include "database.hpp"
#include <iostream>

namespace apotheosis {

database::database() 
    : conn_("host=localhost dbname=apotheosis user=sofa password=w8uSZJZo") {
    if (!conn_.is_open()) {
        throw database_errors("База данных не открылась");
    }
}

int database::add_user(const std::string &name, bool internal_use) {
    pqxx::work txn(conn_);
    pqxx::result check_if_exists = txn.exec_params(
        "SELECT user_id FROM users WHERE name = $1", name);
    
    if (!check_if_exists.empty()) {
        if (internal_use) {
            int id = check_if_exists[0][0].as<int>();
            txn.commit();
            return id;
        }
        txn.abort();
        throw database_errors("Такой человек уже присутствует в бд");
    }
    
    pqxx::result res = txn.exec_params(
        "INSERT INTO users (name) VALUES ($1) RETURNING user_id", name);
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не вышло добавить человека " + name);
    }
    
    int new_id = res[0][0].as<int>();
    txn.commit();
    return new_id;
}

int database::add_task(int contest_number, int task_number, bool internal_use) {
    pqxx::work txn(conn_);
    pqxx::result check_if_exists = txn.exec_params(
        "SELECT task_id FROM tasks WHERE contest_number = $1 AND task_number = $2",
        contest_number, task_number);
    
    if (!check_if_exists.empty()) {
        if (internal_use) {
            int id = check_if_exists[0][0].as<int>();
            txn.commit();
            return id;
        }
        txn.abort();
        throw database_errors("Такое задание уже существует");
    }
    
    pqxx::result res = txn.exec_params(
        "INSERT INTO tasks (contest_number, task_number) VALUES ($1, $2) RETURNING task_id",
        contest_number, task_number);
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не вышло добавить задание " + std::to_string(task_number));
    }
    
    int id = res[0][0].as<int>();
    txn.commit();
    return id;
}

int database::load_code(const std::string &child_name, int task, 
                       const std::string &contest, const std::string &code) {
    int user_id = add_user(child_name, true);
    int task_id = add_task(0, task, true); // contest_number временно 0
    
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params(
        "INSERT INTO solutions(task_id, user_id, code) VALUES($1, $2, $3) RETURNING solution_id", 
        task_id, user_id, pqxx::binarystring(code.data(), code.size()));
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не вышло залить код в базу данных для " + child_name);
    }
    
    int id = res[0][0].as<int>();
    txn.commit();
    return id;
}

int database::load_graph(int solution_id, const std::string &graph) {
    pqxx::work txn(conn_);
    pqxx::result check_if_exists = txn.exec_params(
        "SELECT 1 FROM solutions WHERE solution_id = $1", solution_id);
    
    if (check_if_exists.empty()) {
        txn.abort();
        throw database_errors("Такого решения не существует");
    }
    
    pqxx::result res = txn.exec_params(
        "INSERT INTO graphs(solution_id, graph) VALUES($1, $2) RETURNING graph_id", 
        solution_id, pqxx::binarystring(graph.data(), graph.size()));
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не вышло залить граф в базу данных для решения " + 
                            std::to_string(solution_id));
    }
    
    int id = res[0][0].as<int>();
    txn.commit();
    return id;
}

std::string database::get_graph(int graph_id) {
    pqxx::read_transaction txn(conn_);
    pqxx::result res = txn.exec_params(
        "SELECT encode(graph, 'escape') FROM graphs WHERE graph_id = $1",
        graph_id);
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не нашелся граф " + std::to_string(graph_id));
    }
    
    std::string graph = res[0][0].as<std::string>();
    txn.commit();
    return graph;
}

std::string database::get_solution(int solution_id) {
    pqxx::read_transaction txn(conn_);
    pqxx::result res = txn.exec_params(
        "SELECT encode(code, 'escape') FROM solutions WHERE solution_id = $1",
        solution_id);
    
    if (res.empty()) {
        txn.abort();
        throw database_errors("Не нашлось решение " + std::to_string(solution_id));
    }
    
    std::string solution = res[0][0].as<std::string>();
    txn.commit();
    return solution;
}

bool database::user_exists(const std::string &name) {
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params(
        "SELECT 1 FROM users WHERE name = $1 LIMIT 1", name);
    txn.commit();
    return !res.empty();
}

void database::delete_user(int user_id) {
    pqxx::work txn(conn_);
    txn.exec_params("DELETE FROM users WHERE user_id = $1", user_id);
    txn.commit();
}

pqxx::result database::get_user_solutions(const std::string &user_name) {
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params(
        "SELECT s.solution_id, t.contest_number, t.task_number, s.code "
        "FROM solutions s "
        "JOIN users u ON s.user_id = u.user_id "
        "JOIN tasks t ON s.task_id = t.task_id "
        "WHERE u.name = $1", user_name);
    txn.commit();
    return res;
}

} // namespace apotheosis
