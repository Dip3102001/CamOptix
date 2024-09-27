#include<iostream>
#include<string>
#include<vector>

#include "Graph.hpp"

class Main{
    private:
        std::string *buffer;
        int V;
        std::pair<int,int> source_to_destination;
        std::vector<std::vector<int>> edge;


    public:
        Main(std::string buffer[]);
        void parse_line();
        void print_output();
};