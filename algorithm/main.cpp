#include <iostream>
#include "apotheosis.hpp"
#include "interpreter.hpp"
#include "VF2.hpp"




int main(){
    std::cout<<"Я запустился!\n";
    // apotheosis::Interpreter interpreter;

    // interpreter.start_interpreter();

    apotheosis::Subgraph l("Левый");
    apotheosis::Graph r("Правый");
    int n,m;
    std::cin>>n>>m;
    
    for (int i=0;i<n;i++){
        std::string type;
        std::cin>>type;
        l.add_vertex(type);
        r.add_vertex(type);
    }
    for (int i=0;i<m;i++){
        int a,b;
        std::cin>>a>>b;
        l.add_edge(a,b);
        r.add_edge(a,b);
    }
    
    std::vector<apotheosis::Subgraph> loh{l};
    apotheosis::VF2 algo(loh,r); 
    std::cout<<algo.check()<<'\n';

    
    std::cout<<"Я отработал!\n";
    return 0;
}