#ifndef ERRORS_HPP_
#define ERRORS_HPP_
#include <exception>
#include <iostream>
#include <string>

namespace apotheosis {

struct not_a_graph : std::runtime_error {
    not_a_graph(std::string filepath)
        : std::runtime_error("\"" + filepath + "\"" + " - это не граф!"){};
};

struct unable_to_open : std::runtime_error {
    explicit unable_to_open(const std::string &fname)
        : std::runtime_error("Unable to open file \'" + fname + "\'"){};
};

struct bad_read : std::runtime_error {
    bad_read() : std::runtime_error("Возникла ошибка при чтении"){};
};

}  // namespace apotheosis
#endif