#ifndef JSON_PROCESSOR_HPP_
#define JSON_PROCESSOR_HPP_

#include "database.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <stdexcept>
#include <map>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace apotheosis {

class json_processor {
private:
    database& db_;
    const std::string work_folder_ = "joern_work_folder/test_files";

    // Вспомогательные методы
    void ensure_directory_exists(const std::string& dir_path);
    void save_cpp_file(const std::string& filename, const std::string& code);
    std::string generate_filename(int contest_num, int task_num, int child_id) const;

public:
    explicit json_processor(database& db) : db_(db) {
        ensure_directory_exists(work_folder_);
    }

    struct child_solution {
        int child_id;
        std::string child_name;
        std::string code;
    };

    struct task_data {
        int contest_number;
        int task_number;
        std::vector<ChildSolution> children;
    };

    struct contest_data {
        std::string contest_name;
        std::vector<TaskData> tasks;
    };

    ContestData parse_json(const std::string& json_str);
    void process_contest(const std::string& json_str);
    void process_contest_from_file(const std::string& json_filename);
};

} // namespace apotheosis

#endif
