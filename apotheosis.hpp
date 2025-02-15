#ifndef APOTHEOSIS_HPP_
#define APOTHEOSIS_HPP_

#include <iostream>
#include <exception>

namespace apotheosis{

    struct myerror : std::runtime_error{
        myerror() : std::runtime_error("Шлеп :).\n"){};
    };

}
#endif