cpp#include <iostream>
#include <filesystem>
#include "joern_parse/input-parcer.hpp"

using namespace graph_maker;

void test_func() {
    joern_graph_maker test1;
    files_stack fs1("./joern_work_folder/test_files");
    test1.clear_directory("./joern_work_folder/results");
    test1.make_graph(fs1);
    std::cout << test1.get_result_file_path("1") << std::endl;
}

int main() {
    test_func();
    return 0;
}
