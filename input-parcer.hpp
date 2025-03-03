#ifndef GRAPHGETTER_HPP_
#define GRAPHGETTER_HPP_

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace grath_maker {
class FilesStack {
private:
    std::vector<std::string> files;
    std::vector<std::string> &get_files(const std::string &folder_name);

public:
    int number_of_files_ = 0;
    const std::string folder_;

    FilesStack(const std::string &folder_name)
        : files(std::move(get_files(folder_name))), folder_(folder_name) {
    }

    const std::string &get_file_path(int number);
};

class JoernGraphMaker {
private:
    int number_of_folders;

public:
    JoernGraphMaker() : number_of_folders(0) {
    }

    void create_dot_file(const std::string &file, const std::string &dir);

    void make_graph(FilesStack &stack);
};

}  // namespace grath_maker
#endif
