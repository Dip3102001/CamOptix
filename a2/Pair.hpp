#include<iostream>
#include<sstream>
#include<string>

class Pair{
    private:
        std::string input;
        int first;
        int second;
    public:
        Pair(std::string input);
        void exec();
        std::pair<int,int> get_pair();
};