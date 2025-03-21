#include <filesystem>
#include <iostream>
#include "apotheosis.hpp"
#include "graph-getter.hpp"

int main() {
    std::cout << "Я запустился!\n";

    // Хех

    apotheosis::GraphGetter gg;
    gg.privet();

    throw apotheosis::myerror();

    std::cout << "Я отработал!\n";
    return 0;
}