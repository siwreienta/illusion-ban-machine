#include <iostream>
#include <filesystem>
#include "input-parcer.hpp"

using namespace graph_maker;

void test_func() {
    joern_graph_maker test1;
    files_stack fs1("test_files");
    test1.make_graph(fs1);
    std::cout << test1.get_result_file_path("1") << std::endl;
    //test1.clear_directory("./results");
}

int main() {
    test_func();
    return 0;
}
