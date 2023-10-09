#include "headers.h"

void warp(char* input, bool pr){
    int cnt = 0;
    char* temp = (char*) malloc(4096);
    strcpy(temp, input);
    char* dir;
    char* here;
    while(here = (char*) strtok_r(temp, " ", &temp)){
        dir = here;
        ++cnt;
    }
    int len = strlen(dir);
    if(len && dir[len - 1] == '\n')
        dir[len - 1] = '\0';
    // cnt <= 2
    if(cnt == 1){
        // go to home dir
        strcpy(predir, curdir);
        strcpy(curdir, homedir);
        chdir(curdir);
        getcwd(curdir, N);
        if(pr) 
            printf("%s\n", curdir);
    }
    else{
        strcpy(temp, input);
        cnt = 0;
        while(here = strtok_r(temp, " ", &temp)){
            dir = here;
            int len = strlen(dir);
            if(len && dir[len - 1] == '\n')
                dir[len - 1] = '\0';
            if(cnt){
                // handle ., .., ~, -, any dir
                if(!strcmp("~", dir)){
                    strcpy(predir, curdir);
                    strcpy(curdir, homedir);
                    chdir(curdir);
                }
                else if(!strcmp("-", dir)){
                    if(!strcmp(predir, "-1")){
                        // nothing in history
                        // error handling
                        printf("OLDPWD not set\n");
                        continue;
                    }
                    else{
                        char what[4096];
                        strcpy(what, curdir);
                        strcpy(curdir, predir);
                        strcpy(predir, what);
                        chdir(curdir);
                    }
                }
                // Check if the path is relative or absolute
                else if(dir[0] == '/'){
                    // absolute
                    
                    if(!chdir(dir)){
                        strcpy(predir, curdir);
                        strcpy(curdir, dir);
                        chdir(curdir);
                    }
                    else{
                        printf("Given destination does not exist\n");
                        continue;
                    }
                }
                else{
                    // relative
                    if(dir[0] == '~'){
                        char actual[4096];
                        strcpy(actual, homedir);
                        int j = strlen(homedir);
                        for(int i = 1; i < strlen(dir); ++i)
                            actual[j++] = dir[i];
                        actual[j] = '\0';
                        strcpy(curdir, actual);
                        chdir(curdir);
                    }
                    else{
                        char her[4096];
                        strcpy(her, curdir);
                        strcat(curdir, "/");
                        strcat(curdir, dir);
                        if(!chdir(curdir)){
                            strcpy(predir, her);
                            chdir(curdir);
                        }
                        else{
                            printf("Given destination does not exist\n");
                            continue;
                        }
                    }
                }
                getcwd(curdir, N);
                if(pr)
                    printf("%s\n", curdir);
            }
            ++cnt;
        }
    }
}