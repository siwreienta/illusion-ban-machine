#include <exception>
#include <iostream>
#include <string>

namespace apotheosis {

struct not_a_grath : std::runtime_error {
    not_a_grath(std::string filepath)
        : std::runtime_error("\"" + filepath + "\"" + " - это не граф!"){};
};

struct unable_to_open : std::runtime_error {
    explicit unable_to_open(const std::string &fname)
        : std::runtime_error("Unable to open file \'" + fname + "\'"){};
};

struct invalid_file_format : std::runtime_error {
    invalid_file_format() : std::runtime_error("Invalid file format"){};
};

struct bad_read : std::runtime_error {
    bad_read() : std::runtime_error("Возникла ошибка при чтении"){};
};

struct invalid_command_format : std::runtime_error {
    invalid_command_format() : std::runtime_error("Invalid command format"){};
};

struct unknown_command : std::runtime_error {
    explicit unknown_command(const std::string &cmdname)
        : std::runtime_error(
              "Ты по русски-то пиши. Это: \"" + cmdname + "\" чё такое?"
          ){};
};

struct is_not_register : std::runtime_error {
    explicit is_not_register(const std::string &reg)
        : std::runtime_error("\'" + reg + "\' is not a register"){};
};
}  // namespace apotheosis