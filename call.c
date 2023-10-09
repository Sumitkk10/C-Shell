#include "headers.h"

char* call(char* input){
    int cnt = 0;
    char* temp = (char*) malloc(4096);
    strcpy(temp, input);
    char* command;
    char* here;
    char* last;
    char* forpastevents;
    while(here = strtok_r(temp, " ", &temp)){
        if(!cnt)
            command = here;
        if(cnt == 1)
            forpastevents = here;
        last = here;
        ++cnt;
    }
    // now we just need to check for each command
    int len = strlen(command);
    if(len && command[len - 1] == '\n')
        command[len - 1] = '\0';
    char* printt = (char*) malloc(N);
    strcpy(printt, "-1");
    // different commands here:
    if(!strcmp("warp", command))
        warp(input, 1);
    else if(!strcmp("proclore", command))
        proclore(input);
    else if(!strcmp("pastevents", command)){
        if(cnt == 1)
            print();
        else{
            if(!strcmp("execute", forpastevents))
                execute(input);
            else if(!strcmp("purge\n", forpastevents))
                purge();
            else
                printf("wrong syntax for pastevents\n");
        }
    }
    else if(!strcmp("peek", command))
        peek(input);
    else if(!strcmp("exit", command))
        exit(0);
    else if(!strcmp("seek", command))
        seek(input);
    else if(!strcmp("&", last)){
        // background process
        for(int i = 0; i < strlen(input); ++i){
            if(input[i] == '&'){
                input[i] = '\0';
                break;
            }
        }
        add_bg(input);
    }
    else{
        // we need to use system commands now
        time_t pre = time(NULL);
        char* temp = input;
        if(temp[strlen(temp) - 1] == '\n')
            temp[strlen(temp) - 1] = '\0';
        foreground(temp);
        time_t now = time(NULL);
        int taken = now - pre;
        if(taken > 2){
            // print the name of the command : time taken on the next prompt
            strcpy(printt, strtok_r(temp, " ", &temp));
            strcat(printt, " : ");
            char* tempp = (char*) malloc(N);
            snprintf(tempp, sizeof(tempp), "%d", taken);
            strcat(printt, tempp);
            strcat(printt, "s");
        }
    }
    return printt;
}