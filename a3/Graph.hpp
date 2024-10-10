#include<vector>

class Graph{
    private:
        int V;
        std::vector<std::vector<int>> G;

    public:
        Graph(int V, std::vector<std::vector<int>> G);
        std::vector<int> find_path(int from,int to);
        bool is_node_present(int node);

    private:
        std::vector<int> __bfs__(int from);
};