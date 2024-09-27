#include "Main.hpp"

#include "Parser.hpp"
#include "InvalidInputException.hpp"
#include "NoShortestPathExist.hpp"

Main::Main(std::string buffer[]){
    this->buffer = buffer;
}

void Main::parse_line(){
    Parser p(this->buffer);    
 
    int v = p.parse_line_1();
    this->V = v;
    std::vector<std::pair<int,int>> e = p.parse_line_2();
    std::pair<int,int> source_to_desination = p.parse_line_3();

    std::vector<std::vector<int>> adj_list;

    for(int i=0;i<this->V;i++)
        adj_list.push_back(*(new std::vector<int>()));

    for(std::pair<int,int> pa : e){
        adj_list[pa.first].push_back(pa.second);
        adj_list[pa.second].push_back(pa.first);
    }

    this->edge = adj_list;
    this->source_to_destination = source_to_desination;
}

void Main::print_output(){
    Graph g(this->V,this->edge);
    try{
        std::vector<int> path = 
            g.find_path(this->source_to_destination.first,this->source_to_destination.second);

        for(int i=0;i<path.size();i++){
            if(i != path.size() -1)
                std::cout<<path[i]+1<<"-";
            else 
                std::cout<<path[i]+1<<std::endl;
        }

    }catch(InvalidInputException e){
        std::cout<<e.what()<<std::endl;
    }catch(NoShortestPathExist e){
        std::cout<<e.what()<<std::endl;        
    }
}