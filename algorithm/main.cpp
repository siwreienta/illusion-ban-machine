#include <iostream>
#include "apotheosis.hpp"
#include "interpreter.hpp"



int main(){
    std::cout<<"Я запустился!\n";
    apotheosis::Interpreter interpreter;

    interpreter.start_interpreter();
    
    std::cout<<"Я отработал!\n";
    return 0;
}