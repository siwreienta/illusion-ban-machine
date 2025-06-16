#include "json_processor.hpp"
#include <pqxx/pqxx>
#include <iostream>

namespace apotheosis {

namespace fs = std::filesystem;

void json_processor::ensure_directory_exists(const std::string& dir_path) {
    try {
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
        }
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error("Directory error: " + std::string(e.what()));
    }
}

std::string json_processor::generate_filename(int contest_num, int task_num, int child_id) const {
    std::ostringstream filename;
    filename << work_folder_ << "/"
             << contest_num << "_"
             << task_num << "_"
             << child_id << ".cpp";
    return filename.str();
}

void json_processor::save_cpp_file(const std::string& filename, const std::string& code) {
    try {
        std::ofstream out_file(filename);
        if (!out_file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        out_file << code;
        out_file.close();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save CPP file: " + std::string(e.what()));
    }
}

json_processor::ContestData json_processor::parse_json(const std::string& json_str) {
    try {
        nlohmann::json j = nlohmann::json::parse(json_str);
        ContestData contest;

        contest.contest_name = j["contest_name"];

        for (const auto& task : j["tasks"]) {
            TaskData task_data;
            task_data.contest_number = task["contest_number"];
            task_data.task_number = task["task_number"];

            for (const auto& child : task["children"]) {
                ChildSolution solution;
                solution.child_id = child["child_id"];
                solution.child_name = child["child_name"];
                solution.code = child["code"];
                task_data.children.push_back(solution);
            }

            contest.tasks.push_back(task_data);
        }

        return contest;
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}

void json_processor::process_contest(const std::string& json_str) {
    try {
        ContestData contest = parse_json(json_str);

        for (const auto& task : contest.tasks) {
            for (const auto& child : task.children) {
                std::string filename = generate_filename(
                    task.contest_number, 
                    task.task_number, 
                    child.child_id
                );
                save_cpp_file(filename, child.code);

                int solution_id = db_.load_code(
                    child.child_name,
                    task.task_number,
                    task.contest_number,
                    child.code
                );

                db_.load_graph(
                    solution_id, 
                    filename
                );
            }
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Processing error: " + std::string(e.what()));
    }
}

void json_processor::process_contest_from_file(const std::string& json_filename) {
    try {
        std::ifstream in_file(json_filename);
        if (!in_file.is_open()) {
            throw std::runtime_error("Failed to open JSON file: " + json_filename);
        }

        std::string json_str(
            (std::istreambuf_iterator<char>(in_file)),
            std::istreambuf_iterator<char>()
        );
        in_file.close();

        process_contest(json_str);
    } catch (const std::exception& e) {
        throw std::runtime_error("File processing error: " + std::string(e.what()));
    }
}

} // namespace apotheosis
