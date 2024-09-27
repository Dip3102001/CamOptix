#include<iostream>
#include<exception>

class InvalidInputException : public std::exception{
    public:
        const char* what() const noexcept override;
};