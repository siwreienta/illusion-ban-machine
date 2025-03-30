#include "input-parcer.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace graph_maker {

files_stack::files_stack(const std::string &folder_name)
    : folder_(folder_name) {
    if (!std::filesystem::exists(folder_name)) {
        throw parcerer_errors("Директория не существует: " + folder_name);
    }
    if (!std::filesystem::is_directory(folder_name)) {
        throw parcerer_errors("Это не директория: " + folder_name);
    }
    files = get_files(folder_name);
}

std::vector<std::string> files_stack::get_files(const std::string &folder_name
) {
    std::vector<std::string> files;
    for (const auto &file : std::filesystem::directory_iterator(folder_name)) {
        if (file.is_regular_file() && file.path().extension() == ".cpp") {
            files.push_back(file.path().string());
            number_of_files_++;
        }
    }
    return files;
}

const std::string &files_stack::get_file_path(int number) {
    if (number < 0 || number >= number_of_files_) {
        throw parcerer_errors("Number out of range(");
    } else {
        return files[number];
    }
}

void joern_graph_maker::create_dot_file(
    const std::string &file,
    // const std::string &dir,
    const std::string &id
) {
    std::filesystem::path path_to_file(file);
    std::string command =
        "joern-export --repr=pdg --format=dot --out ./joern_work_folder/results/" + id;
    int result = std::system(command.c_str());
    if (result != 0) {
        throw parcerer_errors("Ошибка при создании .dot для файла: " + file);
    }
    const std::string path_to_dot_folder =
        "./joern_work_folder/results/" + id;
    const std::string path_to_customized_file =
        "./joern_work_folder/results/" + id + "/output.dot";
    customize_graph(path_to_dot_folder, path_to_customized_file);
}

void joern_graph_maker::customize_graph(
    const std::string &input_folder,
    const std::string &output_file
) {
    std::unordered_map<long long, long long> id_to_number;
    std::map<long long, std::string> vertexes;
    std::vector<std::pair<long long, long long>> edges;
    long long number = 0;

    std::vector<std::filesystem::path> files;

    for (const auto& file: std::filesystem::directory_iterator(input_folder)) {
        if (std::filesystem::is_regular_file(file.path())) {
            files.push_back(file.path());
        }
    }
    std::sort(files.begin(), files.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
        std::string a_str = a.filename().string();
        std::string b_str = b.filename().string();
        int first_number = stoi(a_str.substr(0, a_str.find("-")));
        int second_number = stoi(b_str.substr(0, b_str.find("-")));
        return first_number < second_number;
    });

    for (const auto &element: files) {
            bool was_main = parsing_dot_file(
                element.string(), id_to_number, vertexes, edges, number
            );
            if (was_main) break;
    }
    std::ofstream customized_file(output_file);
    if (!customized_file.is_open()) {
        throw std::runtime_error(
            "Ошибка при открытии и создании файла: " + output_file
        );
    }
    customized_file << "digraph " << input_folder << "\n";
    customized_file << number << " " << edges.size() << "\n";
    for (long long i = 0; i < number; i++) {
        customized_file << i << " " << vertexes[i] << "\n";
    }
    for (long long i = 0; i < static_cast<long long>(edges.size()); i++) {
        customized_file << edges[i].first << " " << edges[i].second << "\n";
    }
    customized_file.close();
}

bool joern_graph_maker::parsing_dot_file(
    std::string input_file,
    std::unordered_map<long long, long long> &id_to_number,
    std::map<long long, std::string> &vertexes,
    std::vector<std::pair<long long, long long>> &edges,
    long long &number
) {
    std::ifstream dot_file(input_file);
    if (!dot_file.is_open()) {
        throw std::runtime_error("Ошибка при открытии файла: " + input_file);
    }
    bool is_main = false;
    long long id;
    std::string line;
    std::getline(dot_file, line);
    if (line.find("main") != std::string::npos) {
        is_main = true;
    }
    while (std::getline(dot_file, line)) { 
        if (line.find(" -> ") == std::string::npos) {
            if (line.find('"') == 0) {
                id = stoll(line.substr(1, line.find('"', 1) - 1));
                id_to_number[id] = number;
                int pos_inc = line.find("<") + 1;
                int pos_comma = pos_inc;
                if(line.find(",", pos_inc) == std::string::npos){
                    pos_comma = line.find("]", pos_inc);
                }
                else{
                    pos_comma = line.find(",", pos_inc);
                }
                std::string info = line.substr(pos_inc, pos_comma - pos_inc);
                vertexes[number] = info;
                number++;
            }
        } else {
            long long id_from = stoll(line.substr(3, line.find('"', 3) - 3));
            int pos = line.find(">") + 3;
            long long id_to = stoll(line.substr(pos, line.find('"', pos) - pos));
            edges.emplace_back(id_to_number[id_from], id_to_number[id_to]);
        }
    }
    dot_file.close();
    return is_main;
}


std::string joern_graph_maker::get_result_file_path(const std::string id) {
    return "./joern_work_folder/results/" + id + "/output.dot";
}

void joern_graph_maker::make_graph(files_stack &stack) {
    std::filesystem::path folder_parent(stack.folder_);
    std::string path_to_folder_parent = folder_parent.parent_path().string();
    std::string result_folder = path_to_folder_parent + "/results";
    for (int i = 0; i < stack.number_of_files_; i++) {
        std::string command = "joern-parse " + stack.get_file_path(i);
        int result = std::system(command.c_str());
        if (result != 0) {
            throw parcerer_errors(
                "Ошибка при парсинге файла: " + stack.get_file_path(i)
            );
        }
        std::filesystem::path id_path(stack.get_file_path(i));
        const std::string id = id_path.filename();
        create_dot_file(stack.get_file_path(i), id);
    }
}

void joern_graph_maker::clear_directory(std::string path_to_dir) {
    if (!std::filesystem::exists(path_to_dir)) {
        throw parcerer_errors("Директория не существует: " + path_to_dir);
    }
    if (!std::filesystem::is_directory(path_to_dir)) {
        throw parcerer_errors("Это не директория: " + path_to_dir);
    }
    for (const auto &element :
         std::filesystem::directory_iterator(path_to_dir)) {
        if (std::filesystem::is_regular_file(element.path())) {
            std::filesystem::remove(element.path());
        }
        if (std::filesystem::is_directory(element.path())) {
            clear_directory(element.path().string());
            std::filesystem::remove(element.path());
        }
    }
}

}  // namespace graph_maker
