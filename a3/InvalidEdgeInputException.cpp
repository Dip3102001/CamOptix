#include "InvalidEdgeInputException.hpp"

const char* InvalidEdgeInputException :: what() const noexcept{
    return "Error : Invalid Edge Input";
}