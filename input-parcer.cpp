#include "input-parcer.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace grath_maker {

std::vector<std::string> &FilesStack::get_files(const std::string &folder_name
) {
    std::vector<std::string> files;
    try {
        for (const auto &file :
             std::filesystem::directory_iterator(folder_name)) {
            if (file.is_regular_file() && file.path().extension() == ".cpp") {
                files.push_back(file.path().string());
                number_of_files_++;
            }
        }
    } catch (const std::filesystem::filesystem_error &) {
        std::cerr << "Чет папка поломанная у тебя: " << folder_name << "\n";
    }
}

const std::string &FilesStack::get_file_path(int number) {
    try {
        if (number < 0 || number >= number_of_files_) {
            throw std::runtime_error("Зачем фигню просишь, такого нет элемента"
            );
        } else {
            return files[number];
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}

// тут еще я буду что-то делать, поэтому такое деление
void JoernGraphMaker::create_dot_file(
    const std::string &file,
    const std::string &dir
) {
    try {
        std::filesystem::path path_to_file(file);
        std::string command = "joern-export --out " + dir + "/" +
                              path_to_file.stem().string() + ".dot" +
                              " --repr dot";
        int result = std::system(command.c_str());
        if (result != 0) {
            throw std::runtime_error(
                "господи помоги joernу справиться с созданием .dot"
            );
        }
    } catch (const std::filesystem::filesystem_error &) {
        std::cerr << "Чет странные файлы у тебя " << file << "\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}

void JoernGraphMaker::make_graph(FilesStack &stack) {
    try {
        number_of_folders++;
        std::filesystem::path folder_parent(stack.folder.parent_path());
        std::string new_dir = folder_parent.string() + "/dot_files" +
                              std::to_string(number_of_folders);
        std::filesystem::create_directories(new_dir);
        for (int i = 0; i < stack.number_of_files_; i++) {
            std::string command = "joern-parse " + stack.get_file_path(i);
            int result = std::system(command.c_str());
            if (result != 0) {
                throw std::runtime_error(
                    "господи помоги joernу справиться с парсингом"
                );
            }
            create_dot_file(stack.get_file_path(i), new_dir);
        }
    } catch (const std::filesystem::filesystem_error &) {
        std::cerr << "Чет папка не создалась, хелп " << stack.folder_ << "\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}

}  // namespace grath_maker
