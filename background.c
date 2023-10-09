#include "headers.h"

int totalpro;
bgpro processes[N];

void removepro(int id){
    for(int i = 0; i < totalpro; ++i){
        if(processes[i].pid == id){
            for(int j = i + 1; j < totalpro; ++j){
                processes[j - 1] = processes[j];
                processes[j - 1].proid--;
            }
            --totalpro;
            break;
        }
    }
}

void checkprocess(){
    pid_t pid;
    int status;
    int forestatus;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
        for(int i = 0; i < totalpro; ++i){
            if(pid == processes[i].pid){
                if (WIFSTOPPED(status))
                    processes[i].status = 'T';
                else if (WIFCONTINUED(status))
                    processes[i].status = 'R';
                else if (WIFEXITED(status)){
                    if (pid < 0)
                        printf("%s exited abnormally (%d)\n", processes[i].command, processes[i].pid);
                    int exit = WEXITSTATUS(status);
                    if (exit == 0)
                        printf("%s exited normally (%d)\n", processes[i].command, processes[i].pid);
                    else
                        printf("%s exited abnormally (%d)\n", processes[i].command, processes[i].pid);
                    fflush(stdout);
                    removepro(pid);
                    processes[i].status = 'E';
                }
            }
        }
    }
}

void add_bg(char* input){
    char* temp = (char*) malloc(N);
    strcpy(temp, input);
    char* here = strtok_r(temp, " ", &temp);
    char* store[100];
    int cnt = 0;
    while(here != NULL){
        store[cnt] = (char*) malloc(N);
        strcpy(store[cnt++], here);
        here = strtok_r(NULL, " ", &temp);
    }
    store[cnt] = NULL;

    pid_t pid = fork();
    setpgid(0, 0);
    if(pid < 0){
        printf("process could not be executed\n");
        return;
    }
    else if(!pid){
        pid_t check = getpid();
        if(!(execvp(store[0], store))){
            printf("ERROR : '%s' is not a valid command\n", store[0]);
            return;
        }
    }
    else{
        printf("%d\n", pid);
        processes[totalpro].pid = pid;
        strcpy(processes[totalpro].command, store[0]);
        processes[totalpro].proid = totalpro++;
    }
}