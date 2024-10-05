#include<iostream>
#include<exception>

class InvalidEdgeInputException : public std::exception{
    public:
        const char* what() const noexcept override;
};