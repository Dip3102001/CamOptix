#include "Pair.hpp"
#include "Parser.hpp"

#include<regex>
#include<string>

Parser::Parser(std::string input[]){
    this->input = input;
}

int Parser::parse_line_1(){
    int line_no = 0;
    std::istringstream ss(this->input[line_no]);
    std::vector<std::string> token;

    while(ss.good()){
        std::string substr;
        std::getline(ss,substr,' ');
        token.push_back(substr); 
    }

    return std::stoi(token[1]);
}

std::vector<std::pair<int,int>> Parser::parse_line_2(){
    int line_no = 1;   
    std::istringstream iss(this->input[line_no]);

    std::vector<std::string> tokens;
    while(iss.good()){
        std::string substr;
        std::getline(iss,substr,' ');
        tokens.push_back(substr);
    }

    std::vector<std::pair<int,int>> v;
    std::vector<std::string> toks = this->split(tokens[1].substr(1,tokens[1].size()-2));

    for(std::string tok : toks){
        v.push_back(Pair(tok).get_pair());
    }

    return v;
}

std::pair<int,int> Parser::parse_line_3(){
    int line_no = 2;

    std::istringstream ss(this->input[line_no]);
    std::vector<std::string> token;

    while(ss.good()){
        std::string substr;
        std::getline(ss,substr,' ');
        token.push_back(substr); 
    }

    return {std::stoi(token[1])-1,std::stoi(token[2])-1};
}

std::vector<std::string> Parser::split(std::string input){
    std::regex pattern(R"(<-?\d+,-?\d+>)");
    std::vector<std::string> result;

    auto begin = std::sregex_iterator(input.begin(),input.end(),pattern);
    auto end = std::sregex_iterator();

    for(auto i = begin; i != end;i++){
        std::smatch match = *i;
        result.push_back(match.str());
    }

    return result;
}