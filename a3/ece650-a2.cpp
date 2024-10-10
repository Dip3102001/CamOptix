// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include<iostream>
#include<string>
#include<sstream>

#include "Main.hpp"
#include "InvalidEdgeInputException.hpp"

int main(int argc, char** argv) {
    std::string input;    
    std::string buffer[3];
    int at = 0;

    while (!std::cin.eof()) {        
        try{
            std::getline(std::cin, input);

            buffer[at++] = input;

            if(at == 3){
                Main m(buffer);
                m.parse_line();
                m.print_output();
                at = 0;       
            }
        }catch(InvalidEdgeInputException e){
            std::cout<<e.what()<<std::endl;
        }
    }

    return 0;
}
