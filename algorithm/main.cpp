#include <iostream>
#include "VF2.hpp"
#include "apotheosis.hpp"

int main() {
    std::cout << "Я запустился!\n";

    // std::cout << "Чек 0 и 1: " << apotheosis::check_two_graphs("0.dot", "1.dot")
            //   << '\n';
    std::string output_="output.dot", output1a = "output1a.dot";
    std::cout<< "Чек _ и 1a: "<<apotheosis::check_two_graphs(output_, output1a)<<'\n';
    // std::cout<< "Чек 2a и 1a: "<<apotheosis::check_two_graphs("output2a.dot", "output1a.dot")<<'\n';
    // std::cout<< "Чек _ и 2a: "<<apotheosis::check_two_graphs("output.dot", "output2a.dot")<<'\n';


    std::cout << "Я отработал!\n";
    return 0;
}