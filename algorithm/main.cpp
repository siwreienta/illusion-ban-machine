#include <iostream>
#include "VF2.hpp"
#include "apotheosis.hpp"

int main() {
    std::cout << "Я запустился!\n";

    // std::cout<< "Чек a и b: "<< apotheosis::check_two_graphs("a.dot",
    // "b.dot")<<'\n';
    // std::cout << "Чек 0 и 1: " << apotheosis::check_two_graphs("0.dot", "1.dot")
            //   << '\n';
    std::string output_="output.dot", output1a = "output1a.dot";
    std::cout<< "Чек _ и 1a: "<<apotheosis::check_two_graphs(output_, output1a)<<'\n';
    // std::cout<< "Чек 2a и 1a: "<<apotheosis::check_two_graphs("output2a.dot", "output1a.dot")<<'\n';
    // std::cout<< "Чек _ и 2a: "<<apotheosis::check_two_graphs("output.dot", "output2a.dot")<<'\n';

    // apotheosis::Graph g("Графичек ^_^");

    // g.add_vertex("Тип 0");
    // g.add_vertex("Тип 1");
    // g.add_vertex("Тип 2");
    // g.add_vertex("Тип 3");
    // g.add_vertex("Тип 4");
    // g.add_vertex("Тип 5");
    // g.add_vertex("Тип 6");
    // g.add_vertex("Тип 1");
    // g.add_vertex("Тип 3");

    // // 0 1 2 3
    // // 4 5 8 6
    // g.add_edge(0, 1);
    // g.add_edge(1, 0);
    // g.add_edge(1, 2);
    // g.add_edge(2, 3);
    // g.add_edge(1, 3);
    // g.add_edge(4, 5);
    // g.add_edge(5, 8);
    // g.add_edge(7, 8);
    // g.add_edge(8, 6);
    // g.add_edge(6, 5);
    // g.add_edge(5, 5);
    // g.add_edge(5, 5);
    // g.add_edge(5, 1);
    // g.add_edge(1, 7);
    // g.add_edge(7, 4);
    // g.add_edge(3, 4);
    // g.add_edge(5, 6);
    // g.add_edge(6, 7);
    // g.add_edge(7, 8);

    // apotheosis::Graph g2 = g;
    // g2.add_vertex("Тип 10");
    // g2.add_edge(9, 1);
    // g2.add_edge(3, 9);

    // std::vector<apotheosis::Subgraph> dev = g2.devide_into_subgraphs();
    // std::cout << "Я поделил:\n";
    // std::cout << dev.size() << std::endl;
    // apotheosis::VF2 algo(dev, g);
    // std::cout << algo.check() << '\n';

    std::cout << "Я отработал!\n";
    return 0;
}