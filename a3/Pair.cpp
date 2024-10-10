#include "Pair.hpp"


Pair::Pair(std::string input){
    this->input = input;
    exec();
}

void Pair::exec(){
    std::string winput = this->input.substr(1,this->input.size()-2);
    std::istringstream ss(winput);

    int *arr = new int[2];
    int at = 0;
    while(ss.good()){
        std::string substr;
        std::getline(ss,substr,',');
        arr[at++] = std::stoi(substr);
    }

    this->first = arr[0];
    this->second = arr[1];
}

std::pair<int,int> Pair::get_pair(){
    return std::pair<int,int>(this->first-1,this->second-1);
}
