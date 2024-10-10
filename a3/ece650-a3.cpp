#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include<vector>
#include<cmath>
#include<limits>

#include<unordered_map>

#include<sstream>

#include<cstring>

using namespace std;

int execute(const char* program, int mod){
    int *pp = new int[2];

    if(pipe(pp) == -1){
        // error creating pipes
        cerr<<"Error : Pipe creation failed"<<endl;
    }

    pid_t pid = fork();

    if(pid < 0){
        cerr<<"Error : Fork creation failed"<<endl;
    }

    if(pid == 0){
        // child process
        dup2(pp[1], STDOUT_FILENO);
        close(pp[0]);

        char *args[2];
        args[0] = (char*)program;
        args[1] = NULL;

        execv(args[0],args);
    }else{
        // parent process
        close(pp[1]);
    
        char buffer[128];
        ssize_t read_byte;
        int number;
        while((read_byte = read(pp[0],buffer,sizeof(buffer)-1))>0){
            buffer[read_byte] = '\0';
            number = atoi(buffer);
        }
        return number % mod;
    }

    return -1;
}

string execute_assignment(const char* program, string input){
    int std_in[2];
    int std_out[2];

    if(pipe(std_in) == -1 || pipe(std_out) == -1){
        cerr<<"Error : Pipe creation for assignment-1 execution is failed.."<<endl;
    }

    pid_t pid = fork();


    if(pid < 0){
        cerr<<"Error : Error creating fork of process.."<<endl;
    }else if(pid == 0){
        // child process 
        dup2(std_in[0],STDIN_FILENO);
        dup2(std_out[1],STDOUT_FILENO);
        close(std_in[1]);
        close(std_out[0]);

        char *args[2];

        args[0] = (char*)program;
        args[1] = NULL;

        execv(args[0],args);
    }else{
        // parent process
        close(std_in[0]);
        close(std_out[1]);

        const char* cc_input = input.c_str();
        write(std_in[1], cc_input, strlen(cc_input));
        close(std_in[1]);

        char buffer[128];
        int byte_read;

        ostringstream oss;

        while((byte_read = read(std_out[0],buffer,sizeof(buffer)-1)>0)){
            buffer[byte_read] = '\0';
            oss<<buffer;
        }

        return oss.str();
    }
}

class NoSupportedArgumentException : exception{
    public:
        const char* what() const noexcept{
            return "No Argument Supported Excpetion...";
        }
};

class Argument{
    private:
        unordered_map<string,int> args;
    
    public:
        Argument(int argc, char **argv){
            args["s"] = 10;
            args["n"] = 5;
            args["l"] = 5;
            args["c"] = 20;

            int opt;
            while(opt = getopt(argc, argv, "snlc")){
                switch(opt){
                    case 's':
                        args["s"] = atoi(optarg);
                    break;

                    case 'n':
                        args["n"] = atoi(optarg);
                    break;

                    case 'l':
                        args["l"] = atoi(optarg);
                    break;

                    case 'c':
                        args["c"] = atoi(optarg);
                    break;

                    default:
                        throw NoSupportedArgumentException();
                    break;
                }
            }
                
        }

        Argument(Argument &args){
            this->args = args.args;
        }

        int get(string key){
            return this->args[key];
        }

        void set(string key, int value){
            this->args[key] = value;
        }
};

class LineSegment{
    private:
        pair<int,int> st;
        pair<int,int> en;

        double m;
        double c;

    public:
        LineSegment(pair<int,int> st, pair<int,int> en){
            this->st = st;
            this->en = en;

            this->m = (this->st.first == this->en.first)? INFINITY : 
            (this->st.second - this->en.second)/(this->st.first - this->en.first);

            this->c = (this->st.first == this->en.first)? INFINITY : this->st.second - this->m * this->st.first;
        }

        bool isIntersect(LineSegment l0){
            if(this->m == l0.m){
                if(this->m == INFINITY) return this->st.first == l0.st.first;
                else return this->c == l0.c;
            }else{
                double x,y;

                if(this->m == INFINITY || l0.c == INFINITY){
                    if(this->m == INFINITY){
                        x = this->st.first;
                        y = l0.m * x + l0.c;
                    }else{
                        x = l0.st.first;
                        y = this->m * x + this->c;
                    }
                }else{
                    x = - (this->c - l0.c) / (this->m - l0.m);
                    y = this->m * x + this->c; 
                }

                return this->isBetween(x,y) && l0.isBetween(x,y);
            }
        }

        bool isBetween(int x, int y){
            int x1 = (st.first < en.first) ? st.first : en.first;
            int x2 = (st.first < en.first) ? en.first : st.first;

            int y1 = (st.second < en.second) ? st.second : en.second;
            int y2 = (st.second < en.second) ? en.second : st.second;
        
            return ((x1 <= x && x <= x2) && (y1 <= y && y <= y2));  
        }
};

class Street{
    private:
        string name;
        int size;
        vector<pair<int,int>> ll;

        bool __intersect__(){
            // code for checking intersection will goes here..
            for(int i=0;i<this->ll.size()-1;i++){
                LineSegment l1 = LineSegment(ll.at(i),ll.at(i+1));
                for(int j=i+1;i<this->ll.size()-1;j++){
                    LineSegment l2 = LineSegment(ll.at(j),ll.at(j+1));
                    if(l1.isIntersect(l2))
                        return true;
                }
            }

            return false;
        }

        bool addCheck(pair<int,int> point){
            ll.push_back(point);
            bool result = __intersect__();
            ll.pop_back();
        }

    public:
        Street(string name, int size){
            this->name = name;
            this->size = size;
        }

        bool add(pair<int,int> point){
            if(!addCheck(point)) return false;
            else ll.push_back(point);
            return true;
        }

        string str(){
            string str = "\""+this->name+"\"" + " ";
            
            for(auto[first, second] : this->ll){
                str += "("+to_string(first)+","+to_string(second)+")";
                str += " ";
            }

            return str;
        }
};

class ExhustedIterationException : public exception{
    public:
        const char* what() const noexcept override{
            return "Error : Exhusted 15 consequtative trials for street-coordinate assignment..";
        }
};

class StreetBuilder{
    private: 
        string streetName;
        int number_of_line_segment;
        int c;

        int getRandom(int c){
            int rand = execute("./nrand",c);
            return rand;
        }

    public:
        StreetBuilder(string streetName, int number_of_line_segment, int c){
            this->streetName = streetName;
            this->number_of_line_segment = number_of_line_segment;
            this->c = c;
        }

        Street& build(){
            Street *strt = new Street(this->streetName,this->number_of_line_segment);
            for(int i=0;i<this->number_of_line_segment+1;i++){
                int count = 0;

                while(!strt->add({getRandom(this->c),getRandom(this->c)}) && count < 15){
                    count++;
                }

                if(count < 15) continue;
                else throw ExhustedIterationException();
            }

            return *strt;
        }
};

class Map{
    private:
        vector<Street> streets;
        Argument &argument;

    public:
        Map(Argument args):argument(args){
            for(int i=0;i<this->argument.get("s");i++){
                streets.push_back(StreetBuilder(string(1,'A'+i),this->argument.get("n"),this->argument.get("c")).build());
            }
        }

        void driver(){ 

            ostringstream oss;
            for(int i=0;i<streets.size();i++){
                oss<<"add "<<streets[i].str()<<endl;
            }
            oss<<"gg"<<endl;

            //establish IPC to assignment-1 
            string resp = execute_assignment("./ece650-a1",oss.str());

            // print output from assignment-2
            string output = execute_assignment("./build/ece-650-a2",resp);

            cout<<output;
        }
};

int main (int argc, char **argv) {
    try{
        Argument argument = Argument(argc, argv);
        Map m = Map(argument);
        m.driver();
    }catch(ExhustedIterationException e){
        cerr<<e.what()<<endl;
    }catch(NoSupportedArgumentException e){
        cerr<<e.what()<<endl;
    }

    return 0;
}