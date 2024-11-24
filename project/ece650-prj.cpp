#include<memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#include<iostream>

#include<vector>
#include<string>
#include<regex>

#include<pthread.h>
#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include<cmath>
#include<csignal>

#define ld long double

using namespace std;

vector<vector<int>> glb(3);
string label[3] = {
    "CNF-SAT-VC",
    "APPROX-VC-1",
    "APPROX-VC-2"
};
vector<ld> times(3,5);


const int n_threads = 3;
pthread_t threads_id[n_threads];


void sig_handler(int signum){
    for(int i=0;i<n_threads;i++)
        pthread_cancel(threads_id[i]);
}

bool getbit(int of, int at){
    return of>>at & 1;
}

vector<int> optim_solve(int V, int m, vector<pair<int,int>> G){
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    int bin_encoding_sz = (int)ceil(log2(V));
    int bin_encoding_sz_m = (int)ceil(log2(m));

    Minisat::Lit l[V][m];
    Minisat::Lit y[bin_encoding_sz][m];

    Minisat:: Lit x[V][bin_encoding_sz_m];
    Minisat:: Lit v[V];

    for(int i=0;i<V;i++){
        for(int j=0;j<m;j++){
            l[i][j] = Minisat::mkLit(solver->newVar());
        }
    }

    for(int i=0;i<bin_encoding_sz;i++){
        for(int j=0;j<m;j++){
            y[i][j] = Minisat::mkLit(solver->newVar());
        }
    }

    for(int i=0;i<V;i++){
        for(int j=0;j<bin_encoding_sz_m;j++){
            x[i][j] = Minisat::mkLit(solver->newVar());
        }
        v[i] = Minisat::mkLit(solver->newVar());
    }

    // adding first condition
    for(int i=0;i<m;i++){
        Minisat::vec<Minisat::Lit> add_tmp;
        for(int j=0;j<V;j++){
            add_tmp.push(l[j][i]);
        }
        solver->addClause(add_tmp);
    }

    // adding third condition
    for(int i=0;i<m;i++){
        for(int j=0;j<V;j++){
            for(int k=0;k<bin_encoding_sz;k++){
                if(getbit(j,k)){
                    solver->addClause(~l[j][i],y[k][i]);
                }else{
                    solver->addClause(~l[j][i],~y[k][i]);
                }
            }
        }
    }

    // adding forth condition
    for(const auto &pair:G){
        
        auto first = pair.first;
        auto second = pair.second;

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

vector<int> vc_0(int V, vector<pair<int,int>> G){
    vector<int> ans;
    for(int i=1;i<=V;i++){
        ans = optim_solve(V,i,G);
        if(ans.size() == 0) continue;
        else{
            sort(ans.begin(),ans.end());
            return ans;
        }
    }
    return ans;
}

// adding node having hightest cardinality
vector<int> vc_1(int V, vector<pair<int,int>> G){ 

    auto find_max_index = [](vector<int> &c){
        int max_index = 0;
        for(int i=1;i<(int)c.size();i++){
            if(c[max_index] < c[i])
                max_index = i;
        }

        return max_index;
    };

    auto isEmpty = [](vector<bool> &v){
        for(int i=0;i<(int)v.size();i++)
            if(!v[i]) return true;
        return false;
    };
    
    vector<int> ans;
    vector<int> c(V,0);

    vector<bool> isRemoved(G.size(),false);

    while(isEmpty(isRemoved)){
        for(int i=0;i<(int)G.size();i++){
            if(!isRemoved[i]){
                c[G[i].first-1]++;
                c[G[i].second-1]++;
            }            
        }

        int index = find_max_index(c);
        ans.push_back(index+1);

        for(int i=0;i<(int)G.size();i++){
            if(G[i].first-1 == index || G[i].second-1 == index)
                isRemoved[i] = true;
        }

        for(int i=0;i<V;i++)
            c[i] = 0;
    } 

    sort(ans.begin(),ans.end());
    return ans;
}

vector<int> vc_2(int V, vector<pair<int,int>> G){

    vector<int> ans;
    vector<bool> isRemoved(G.size(),false);

    auto isEmpty = [](vector<bool> &v){
        for(int i=0;i<(int)v.size();i++)
            if(!v[i]) return true;
        return false;
    };

    auto getEdgeIndex = [](vector<bool> &v){
        for(int i=0;i<(int)v.size();i++)
            if(!v[i]) return i;
        return -1;
    };

    while(isEmpty(isRemoved)){
        int edgeIndex = getEdgeIndex(isRemoved);

        ans.push_back(G[edgeIndex].first);
        ans.push_back(G[edgeIndex].second);

        for(int i=0;i<(int)G.size();i++){
            if(G[i].first == G[edgeIndex].first || G[i].second == G[edgeIndex].first){
                isRemoved[i] = true;
            }if(G[i].first == G[edgeIndex].second || G[i].second == G[edgeIndex].second){
                isRemoved[i] = true;
            }
        }
    } 

    sort(ans.begin(),ans.end());
    return ans;
}

vector<int> (*CNF[])(int, vector<pair<int,int>>) = {
    vc_0,
    vc_1,
    vc_2
};

struct vals{
    int at;
    int V;
    vector<pair<int, int>> &G;

    vals(int at, int V, vector<pair<int, int>> &G) 
        : at(at), V(V), G(G) { 
    }
};

void print(int at){
    vector<int> v = glb[at];

    cout<<label[at]<<": ";
    for(int i=0;i<(int)v.size();i++){
        cout<<v[i];
        if(i != (int)v.size()-1)
            cout<<","; 
    }

    cout<<endl;
}

void print_time(int at){
    cout<<label[at]<<": "<<times[at];
    cout<<endl;
}

void print_approx_ratio(int at){
    cout<<label[at]<<": "<<(ld)glb[at].size() / glb[0].size();
    cout<<endl;
}

void* func(void *v){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

    vals *va = reinterpret_cast<vals*>(v);

    int at = va->at;
    int V = va->V;
    vector<pair<int,int>> G = va->G;

    clockid_t clock_id;
    struct timespec ts;

    pthread_getcpuclockid(pthread_self(), &clock_id);
    vector<int> ans = CNF[at](V,G);
    clock_gettime(clock_id,&ts);
    
    times[at] = ts.tv_sec + ts.tv_nsec / 1e9;

    glb[at] = ans;
    
    return 0;
}

// parsing
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
    
    signal(SIGALRM, sig_handler);

    getline(cin,first);
    getline(cin,second);

    int N = parseFirstLine(first);
    vector<pair<int,int>> G = parseSecondLine(second);
 
    for(int i=0;i<n_threads;i++){
        vals *v = new vals(i,N,G); 
        pthread_create(&threads_id[i],nullptr,func,v);
    }

    alarm(5);

    for(int i=0;i<n_threads;i++){
        pthread_join(threads_id[i],nullptr);
    }

    bool flag_approx = false;
    bool flag_time = false;
    

    int opt;
    while ((opt = getopt(argc, argv, "at")) != -1) {
        switch (opt) {
        case 'a':
            flag_approx = true;
            break;
        case 't':
            flag_time = true;
            break;
        case '?':
            fprintf(stderr, "Usage: %s [-a] [-t]\n", argv[0]);
            fprintf(stderr, "-a : for getting approximation ratio");
            fprintf(stderr, "-t : for getting time");
            exit(EXIT_FAILURE);
        }
    }    

    if(!flag_approx && !flag_time){
        for(int i=0;i<n_threads;i++)
            print(i);
    }else{
        if(flag_approx){
            for(int i=0;i<n_threads;i++)
                print_approx_ratio(i);
        }if(flag_time){
            for(int i=0;i<n_threads;i++)
                print_time(i);
        }
    }

    return 0;
}