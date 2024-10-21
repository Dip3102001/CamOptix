#include<cstdio>

#include <iostream>
#include <unistd.h>
#include <csignal>

#include <cstring>
#include <vector>

using namespace std;

vector<pid_t> child_process;
 

void signalHandler(int signal){
    for(pid_t child : child_process)
        kill(child,SIGTERM);
    exit(signal);
}


int main (int argc, char **argv) {

    signal(SIGINT, signalHandler);

    string input;

    int rgen_pipe[2];

    if(pipe(rgen_pipe) == -1 ){
        cerr<<"Error : Pipe Creation is Failed..."<<endl;
        exit(1);
    }else{
        pid_t pid_1 = fork();

        if(pid_1 < 0){
            cerr<<"Error : Forked Failed..."<<endl;
            exit(1);
        }else if(pid_1 == 0){
            // child process
            dup2(rgen_pipe[1],STDOUT_FILENO);
            close(rgen_pipe[0]);

            char *args[argc+1];

            args[0] = (char*)"./rgen";
            for(int i=1;i<argc;i++)
                args[i] = argv[i];
            args[argc] = NULL;

            execv(args[0],args);
        }else{

            int ass_1_pipe[2];

            if(pipe(ass_1_pipe) == -1){
                cerr<<"Error : Pipe creation is Failed..."<<endl;
                exit(1);
            }else{
                pid_t pid_2 = fork();

                if(pid_2 < 0){
                    cerr<<"Error : Forked Failed..."<<endl;
                    exit(1);
                }else if(pid_2 == 0){
                    // child process
                    dup2(rgen_pipe[0],STDIN_FILENO);
                    close(rgen_pipe[1]);

                    dup2(ass_1_pipe[1],STDOUT_FILENO);
                    close(ass_1_pipe[0]);

                    char *args[2];
                    args[0] = (char*)"../ece650-a1.py";
                    args[1] = NULL;

                    execv(args[0],args);
                }else{
                    pid_t pid_3 = fork();

                    if(pid_3 < 0){
                        cerr<<"Error : Fork creation failed..."<<endl;
                        exit(1);
                    }else if(pid_3 == 0){
                        // child process
                        dup2(ass_1_pipe[0],STDIN_FILENO);
                        close(ass_1_pipe[1]);
                        close(rgen_pipe[0]);
                        close(rgen_pipe[1]);

                        char *args[2];
                        args[0] = (char*)"./ece650-a2";
                        args[1] = NULL;

                        execv(args[0],args);
                    }else{
                        // parent process
                        child_process.push_back(pid_1);
                        child_process.push_back(pid_2);
                        child_process.push_back(pid_3);

                        close(rgen_pipe[0]);
                        close(rgen_pipe[1]);
                        close(ass_1_pipe[0]);

                        string input;
                        while(true){
                            getline(cin,input);
                            if(input.empty()) break;
                            
                            input += '\n';

                            const char *cc_input = input.c_str();
                            write(ass_1_pipe[1],cc_input,strlen(cc_input));
                        }

                        close(ass_1_pipe[1]);

                        for(pid_t child : child_process)
                            kill(child,SIGTERM);
                    }   
                }
            }     
        }
    }

    return 0;
}