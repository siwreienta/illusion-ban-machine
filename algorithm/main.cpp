#include <iostream>
#include "VF2.hpp"
#include "apotheosis.hpp"

int main() {
    #ifdef APOTHEOSIS_DEBAG
    std::cout << "АПОФЕОЗ ДЕБАГА\n";
    #endif
    std::cout << "Я запустился!\n";

    // std::cout << "Чек 0 и 1: " << apotheosis::check_two_graphs("0.dot", "1.dot")
            //   << '\n';
    std::string output_="output.dot", output1a = "output1a.dot", output2a = "output2a.dot";
    // std::cout<< "Чек _ и 1a: "<<apotheosis::check_two_graphs(output_, output1a)<<'\n';
    // std::cout<< "Чек 2a и 1a: "<<apotheosis::check_two_graphs(output2a, output1a)<<'\n';
    // std::cout<< "Чек _ и 2a: "<<apotheosis::check_two_graphs("output.dot", "output2a.dot")<<'\n';
    // auto res = apotheosis::check_two_graphs(output_, output_);
    // std::cout<< "Чек new: "<<res<<'\n';
    // res = apotheosis::check_two_graphs_old(output_, output_);
    // std::cout<< "Чек old: "<<res<<'\n';

    // for (int i = 4; i<=12;i++){
    //     for (int j = i+1; j<=12; j++){
    //         std::string first = "joern_work_folder/results/"+std::to_string(i)+".dot";
    //         std::string second = "joern_work_folder/results/"+std::to_string(j)+".dot";
    //         std::cout<<i<<" "<<j<<std::endl;
    //         auto res = apotheosis::check_two_graphs(first, second);
    // std::cout<< "Чек new: "<<res<<'\n';
    // res = apotheosis::check_two_graphs_old(first, second);
    // std::cout<< "Чек old: "<<res<<'\n';
            
    //     }
    // }
    apotheosis::check_all_in_dir("joern_work_folder/results");

    std::cout << "Я отработал!\n";
    return 0;
}