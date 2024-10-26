#include<iostream>
#include<fstream>

#include <unistd.h>
#include <sys/wait.h>

#include<vector>
#include<cmath>
#include<limits>

#include<unordered_map>

#include<sstream>

#include<cstring>

#include<thread>
#include<chrono>

using namespace std;

// defining exception...
class ExhustedIterationException : public exception{
    public:
        const char* what() const noexcept override{
            return "Error : Exhusted 25 consequtative trials for street-coordinate assignment...";
        }
};

class NoSupportedArgumentException : public exception{
    public:
        const char* what() const noexcept override{
            return "Error : No Argument Supported Excpetion...";
        }
};

class ArgumentAbuse : public exception{
    public:
        const char* what() const noexcept override{
            return "Error : Incorrect value passed to argument...";
        }
};

// get random number
int rand(){
    ifstream urand("/dev/urandom",ios::in|ios::binary);

    if(!urand){
        cerr<<"Error : Unable to open /dev/urandom."<<endl;
        return 1;
    }else{
        int random_number;
        urand.read((char*)(&random_number),sizeof(random_number));

        urand.close();
        return random_number;
    }

}

int rand(int from, int to){
    int number = abs(rand()) % 100000007;

    int mod = to - from + 1;
    return from + (number % mod);
}

// arument parser
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
            while((opt = getopt(argc, argv, "s:n:l:c:")) != -1){
                // chage required to check for range
                switch(opt){
                    case 's':
                        args["s"] = atoi(optarg);
                        if(args["s"] < 2 || args["s"] > 10) throw ArgumentAbuse();
                    break;

                    case 'n':
                        args["n"] = atoi(optarg);
                        if(args["n"] < 1) throw ArgumentAbuse();
                    break;

                    case 'l':
                        args["l"] = atoi(optarg);
                        if(args["l"] < 5) throw ArgumentAbuse();
                    break;

                    case 'c':
                        args["c"] = atoi(optarg);
                        if(args["c"] < 1) throw ArgumentAbuse();
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

        double __round__(double x){
            return round((x * 1e2)) * 1e-2;
        }

    public:
        LineSegment(pair<int,int> st, pair<int,int> en){
            this->st = st;
            this->en = en;

            this->m = (this->st.first == this->en.first)? INFINITY : 
            (double)(this->st.second - this->en.second)/(double)(this->st.first - this->en.first);

            this->c = (this->st.first == this->en.first)? INFINITY : 
            this->st.second - this->m * this->st.first;
        }

        bool isIntersect(LineSegment l0){
            if(this->m == l0.m){
                if(this->m == INFINITY) return this->st.first == l0.st.first;
                else return this->c == l0.c;
            }else{
                double x,y;

                if(this->m == INFINITY || l0.m == INFINITY){
                    if(this->m == INFINITY){
                        x = this->st.first;
                        y = l0.m * x + l0.c;
                    }else{
                        x = l0.st.first;
                        y = this->m * x + this->c;
                    }
                }else{
                    x = - ((this->c - l0.c) / (this->m - l0.m));
                    y = this->m * x + this->c; 
                }

                x = __round__(x);
                y = __round__(y);

                return this->isBetween(x,y) || l0.isBetween(x,y);
            }
        }

        bool isBetween(double x, double y){
            double x1 = (st.first < en.first) ? st.first : en.first;
            double x2 = (st.first < en.first) ? en.first : st.first;

            double y1 = (st.second < en.second) ? st.second : en.second;
            double y2 = (st.second < en.second) ? en.second : st.second;
        
            return ((x1 < x && x < x2) && (y1 < y && y < y2));  
        }
};

class Street{
    private:
        string name;
        int size;
        vector<pair<int,int>> ll;

        bool __intersect__(){
            // code for checking intersection will goes here..
            for(int i=0;i<(int)this->ll.size()-1;i++){
                LineSegment l1 = LineSegment(ll.at(i),ll.at(i+1));
                for(int j=i+1;j<(int)this->ll.size()-1;j++){
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
            
            return !result;
        }

    public:
        Street(string name, int size){
            this->name = name;
            this->size = size;
        }

        bool add(pair<int,int> point){
            if(addCheck(point)) ll.push_back(point);
            else return false;
            return true;
        }

        string str(){
            string str = "\""+this->name+"\"" + " ";
            
            for(pair<int,int > p : this->ll){
                int first = p.first;
                int second = p.second;
                str += "("+to_string(first)+","+to_string(second)+")";
                str += " ";
            }

            return str;
        }
};

class StreetBuilder{
    private: 
        string streetName;
        int number_of_line_segment;
        int c;

        int getRandom(int c){
            int __rand = rand();
            return __rand % c;
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

                while(!strt->add({getRandom(this->c),getRandom(this->c)}) && count < 25){
                    count++;
                }

                if(count < 25) continue;
                else{
                    throw ExhustedIterationException();
                } 
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
            for(int i=0;i<rand(2,this->argument.get("s"));i++){
                streets.push_back(StreetBuilder(string(1,'A'+i),rand(1,this->argument.get("n")),this->argument.get("c")).build());
            }
        }

        void driver(){ 
            for(int i=0;i<(int)streets.size();i++){
                cout<<"add "<<streets[i].str()<<endl;
            }
            cout<<"gg"<<endl;
        }

        void destroy(){
            for(int i=0;i<streets.size();i++)
                cout<<"rm"<<" "<<"\""<<(char)('A'+i)<<"\""<<endl;
        }
};


int main(int argc, char** argv){

    try{
        Argument argument(argc, argv);
        while(true){
            try{
                Map m(argument);
                m.driver();
                this_thread::sleep_for(chrono::seconds(rand(5,argument.get("l"))));
                m.destroy();
            }catch(ExhustedIterationException &e){
                cerr<<e.what()<<endl;
            }
        }
    }catch(NoSupportedArgumentException &e){
        cerr<<e.what()<<endl;
        throw e;
    }    

    return 0;
}