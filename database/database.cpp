#include "database.hpp"
#include <iostream>
#include <pqxx/pqxx>

namespace apotheosis {

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

int database::load_code(const std::string &child_name, int task, int contest, const std::string &code) {
    int user_id = add_user(child_name, true);
    int task_id = add_task(contest, task, true);
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
        throw database_errors("Не вышло залить граф в базу данных для решения " + std::to_string(solution_id));
    }
    
    int id = res[0][0].as<int>();
    txn.commit();
    return id;
}

const std::string database::get_graph(int graph_id) {
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

const std::string database::get_solution(int solution_id) {
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

}  // namespace apotheosis
