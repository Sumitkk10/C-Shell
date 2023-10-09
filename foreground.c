#include "headers.h"

void foreground(char* input){
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
    if(pid < 0){
        printf("process could not be executed\n");
        return;
    }
    else{
        if(!pid){
            if(execvp(store[0], store) == -1){
                printf("ERROR : '%s' is not a valid command\n", store[0]);
                return;
            }
        }
        else{
            int wait;
            waitpid(pid, &wait, WUNTRACED);
        }
    }
}