#include "headers.h"

void add(char* input){
    char location[N];
    strcpy(location, homedir);
    int x = strlen(location);
    char here[15] = "/pastevents.txt";
    for(int i = 0; i < 15; ++i)
        location[x++] = here[i];
    location[x] = '\0';
    FILE *fp = fopen(location, "r");
    if(fp == NULL){
        printf("ok\n");
        return;
    }
    char* line = NULL;
    char last[N];
    int cnt = 0;
    size_t len = 0;
    char store[20][N];
    while(getline(&line, &len, fp) != -1){
        strcpy(store[cnt], line);
        ++cnt;
        strcpy(last, line);
    }
    if(strcmp(last, "") && !strcmp(input, last)){
        fclose(fp);
        return;
    }
    fclose(fp);
    if(cnt >= 15){
        // remove the first line
        // and add in the end
        fp = fopen(location, "w");
        fclose(fp);
        fp = fopen(location, "a");
        for(int i = 1; i < cnt; ++i)
            fputs(store[i], fp);
        fputs(input, fp);
    }
    else{
        fp = fopen(location, "a");
        fputs(input, fp);
    }
    fclose(fp);
}

void execute(char* input){
    char location[N];
    strcpy(location, homedir);
    int x = strlen(location);
    char here[15] = "/pastevents.txt";
    for(int i = 0; i < 15; ++i)
        location[x++] = here[i];
    location[x] = '\0';
    FILE *fp = fopen(location, "r");
    if(fp == NULL){
        printf("ok\n");
        return;
    }
    char* line = NULL;
    size_t len = 0;
    int cnt = 0;
    char store[20][N];
    while(getline(&line, &len, fp) != -1){
        strcpy(store[cnt], line);
        ++cnt;
    }
    char* temp;
    temp = strtok(input, " ");
    temp = strtok(NULL, " ");
    temp = strtok(NULL, " ");
    if(temp == NULL){
        printf("Provide a command in history to execute");
        fclose(fp);
        return;
    }
    int num = atoi(temp), which = 1;
    for(int i = cnt - 1; i >= 0; --i){
        if(which == num){
            call(store[i]);
        }
        ++which;
    }
    fclose(fp);
}

void print(){
    char location[N];
    strcpy(location, homedir);
    int x = strlen(location);
    char here[15] = "/pastevents.txt";
    for(int i = 0; i < 15; ++i)
        location[x++] = here[i];
    location[x] = '\0';
    FILE *fp = fopen(location, "r");
    if(fp == NULL){
        printf("Error\n");
        return;
    }
    size_t len = 0;
    char* line = NULL;
    while(getline(&line, &len, fp) != -1)
        printf("%s", line);
    fclose(fp);
}
 
void purge(){
    char location[N];
    strcpy(location, homedir);
    int x = strlen(location);
    char here[15] = "/pastevents.txt";
    for(int i = 0; i < 15; ++i)
        location[x++] = here[i];
    location[x] = '\0';
    FILE *fp = fopen(location, "w");
    if(fp == NULL){
        printf("ok\n");
        return;
    }
    fclose(fp);
}