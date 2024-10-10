#include "Graph.hpp"
#include "InvalidInputException.hpp"
#include "NoShortestPathExist.hpp"
#include "InvalidEdgeInputException.hpp"

#include<queue>
#include<stack>

Graph::Graph(int V, std::vector<std::vector<int>> G){
    this->V = V;
    this->G = G;
}

bool Graph::is_node_present(int from){
    return  0 <= from && from < this->V;
}

std::vector<int> Graph::find_path(int from, int to){
    if(this->is_node_present(from) && this->is_node_present(to)){
        std::vector<int> p = this->__bfs__(from);
        if(p[to] == -1) throw NoShortestPathExist();
        else{
            std::stack<int> s;
            std::vector<int> path;

            int at = to;
            while(p[at] != at){
                s.push(at);
                at = p[at];
            }
            s.push(from);

            while(s.size() != 0){
                path.push_back(s.top());
                s.pop();
            }

            return path;
        }
    }else{
        throw InvalidInputException();       
    }
}

std::vector<int> Graph::__bfs__(int from){
    std::queue<int> q;
    std::vector<bool> isVisited(this->V,false);
    std::vector<int> p(this->V,-1);

    isVisited[from] = true;
    q.push(from);
    p[from] = from;
    
    while(q.size() != 0){
        int at = q.front();
        q.pop();
        
        for(int n : this->G[at]){
            if(!isVisited[n]){
                isVisited[n] = true; 
                q.push(n);            
                p[n] = at;
            }
        }
    }

    return p;
}