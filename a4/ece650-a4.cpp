#include<memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#include<iostream>

#include<vector>
#include<string>
#include<regex>

using namespace std;

vector<int> solve(int V, int m, vector<pair<int,int>> G){
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    Minisat::Lit l[V][m];

    for(int i=0;i<V;i++){
        for(int j=0;j<m;j++){
            l[i][j] = Minisat::mkLit(solver->newVar());
        }
    }

    // adding first condition
    for(int i=0;i<m;i++){
        Minisat::vec<Minisat::Lit> add_tmp;
        for(int j=0;j<V;j++){
            add_tmp.push(l[j][i]);
        }
        solver->addClause(add_tmp);
    }

    // adding second condition
    for(int i=0;i<m;i++){
        for(int j=0;j<V-1;j++){
            for(int k=j+1;k<V;k++){
                solver->addClause(~l[j][i],~l[k][i]);
            }
        }
    }

    // adding third condition
    for(int i=0;i<m-1;i++){
        for(int j=i+1;j<m;j++){
            for(int k=0;k<V;k++){
                solver->addClause(~l[k][i],~l[k][j]);
            }
        }
    }

    // adding forth condition
    for(auto[first,second] : G){
        Minisat::vec<Minisat::Lit> add_tmp;
        for(int i=0;i<m;i++){
            add_tmp.push(l[first-1][i]);
            add_tmp.push(l[second-1][i]);
        }
        solver->addClause(add_tmp);
    }

    bool res = solver->solve();
    
    vector<int> ans;

    if(res){
        for(int i=0;i<m;i++){
            for(int j=0;j<V;j++){
               if(Minisat::toInt(solver->modelValue(l[j][i])) == 0){
                    ans.push_back(j+1);
               } 
            }
        }
    }

    return ans;
}

pair<int,int> getPair(string str){
    string pattern = R"(\d+)";
    regex regexPattern(pattern);

    auto begin = sregex_iterator(str.begin(),str.end(),regexPattern);
    int first, second;

    first = stoi((*begin).str());
    second = stoi((*++begin).str());

    return {first,second};
}

int parseFirstLine(string str){
    return stoi(str.substr(2,str.length()-2));
}

vector<pair<int,int>> parseSecondLine(string str){
    string pattern = R"(<\d+,\d+>)";

    regex regexPattern(pattern);

    auto begin = sregex_iterator(str.begin(),str.end(),regexPattern);
    auto end = sregex_iterator();

    vector<pair<int,int>> vector;

    for(sregex_iterator i = begin; i != end; ++i){
        vector.push_back(getPair((*i).str()));
    }   
    
    return vector;
}



int main(int argc, char **argv) {
    string first, second;
    
    getline(cin,first);
    getline(cin,second);

    int N = parseFirstLine(first);
    vector<pair<int,int>> G = parseSecondLine(second);

    for(int i=1;i<=N;i++){
        vector<int> ans = solve(N,i,G);
        if(ans.size() == 0) continue;
        else{
            sort(ans.begin(),ans.end());
            for(int v : ans){
                cout<<v<<" ";
            }
            cout<<endl;
            break;
        }
    }

    return 0;
}