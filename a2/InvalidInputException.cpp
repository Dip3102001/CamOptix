#include "InvalidInputException.hpp"

const char* InvalidInputException::what() const noexcept{
    return "Error: Invalid input is given";
}