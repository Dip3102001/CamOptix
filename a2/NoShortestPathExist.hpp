#include<iostream>
#include<exception>

class NoShortestPathExist : public std::exception{
    public:
        const char* what() const noexcept override;
};