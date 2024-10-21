// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include<iostream>
#include<string>
#include<sstream>

#include "Main.hpp"
#include "InvalidEdgeInputException.hpp"


bool startWith(std::string input, char ch){
    return input[0] == ch;
}

int main(int argc, char** argv) {
    std::string input;    
    std::string buffer[3];

    while (!std::cin.eof()) {
        try{
            std::getline(std::cin,input);

            if(startWith(input,'V')){
                buffer[0] = input;
                std::cout<<input<<std::endl;
            }else if(startWith(input,'E')){
                buffer[1] = input;
                std::cout<<input<<std::endl;
            }else if(startWith(input,'s')){
                buffer[2] = input;
                Main m(buffer);
                m.parse_line();
                m.print_output();
                std::cout.flush();
            }
        }catch(InvalidEdgeInputException &e){
            std::cout<<e.what()<<std::endl;
        }
    }

    return 0;
}
