#include<iostream>
#include<sstream>
#include<string>
#include<vector>

class Parser{
    private:
        std::string *input;
        
    public:
        Parser(std::string input[]);
        int parse_line_1();
        std::vector<std::pair<int,int>> parse_line_2();
        std::pair<int,int> parse_line_3();
        std::vector<std::string> split(std::string input);
};