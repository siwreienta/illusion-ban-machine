#ifndef INPUTPARCER_HPP_
#define INPUTPARCER_HPP_

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "database.hpp"

namespace graph_maker {

class parcerer_errors : public std::exception {
private:
    std::string message;

public:
    parcerer_errors(const std::string &msg) : message(msg) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class files_stack {
private:
    std::vector<std::string> files;
    std::vector<std::string> get_files(const std::string &folder_name);

public:
    int number_of_files_ = 0;
    const std::string folder_;

    files_stack(const std::string &folder_name);
    const std::string &get_file_path(int number);
    
    void parse_filename(const std::string& filename, 
                       int& contest_num, 
                       int& task_num, 
                       int& child_id) const;
};

class joern_graph_maker {
private:
    apotheosis::database& db_;

public:
    explicit joern_graph_maker(apotheosis::database& db) : db_(db) {}

    void create_dot_file(const std::string &file, const std::string &id);
    void make_graph(files_stack &stack);
    std::string get_result_file_path(int id);
    void clear_directory(std::string path_to_dir);
    
    bool parsing_dot_file(
        std::string input_file,
        std::unordered_map<long long, long long> &id_to_number,
        std::map<long long, std::string> &vertexes,
        std::vector<std::pair<long long, long long>> &edges,
        long long &number
    );

    void customize_graph(
        const std::string &input_file,
        const std::string &output_file
    );
    
    void save_graph_to_db(const std::string& dot_file_path, 
                         int contest_num, 
                         int task_num, 
                         int child_id);
};

}  // namespace graph_maker
#endif
