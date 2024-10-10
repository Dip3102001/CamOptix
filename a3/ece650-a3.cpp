#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include<vector>
#include<cmath>
#include<limits>
#include<cstring>

#include<unordered_map>

#include<sstream>

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

string execute_assignment_1(const char* program, string input){
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

        args[0] = program;
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
            buffer[read_byte] = '\0';
            oss<<buffer;
        }

        return oss.str();
    }
}

string execute_assignment_2(const *program, string input){
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

        args[0] = program;
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
            buffer[read_byte] = '\0';
            oss<<buffer;
        }

        return oss.str();
    }
}

class Street{
    private:
        pair<Integer,Integer> ll[];
};

class StreetBuilder{

};



int main (int argc, char **argv) {
    int number = execute("./nrand",23);
    cout<<number<<endl;
    return 0;
}
