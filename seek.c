#include "headers.h"
bool d, f, e, ok_seek, first;
int tot;
bool directory = 0;
char path_seek[N], search[N], store[201];

void init_seek(){
    d = 0;
    f = 0;
    e = 0;
    tot = 0;
    directory = 0;
    ok_seek = 1;
    first = 0;
}

void dnc_seek(char* input){ // divide and conquer 
    if (!(strcmp("-def", input)) || !(strcmp("-efd", input)) || !(strcmp("-fde", input)) || !(strcmp("-dfe", input)) || !(strcmp("-edf", input)) || !(strcmp("-fed", input))){
        d = 1;
        f = 1;
        e = 1;
    }
    else if(!(strcmp("-de", input)) || !(strcmp("-ed", input))){
        d = 1;
        e = 1;
    }
    else if(!(strcmp("-df", input)) || !(strcmp("-fd", input))){
        d = 1;
        f = 1;
    }
    else if(!(strcmp("-ef", input)) || !(strcmp("-fe", input))){
        e = 1;
        f = 1;
    }
    else if (!(strcmp("-d", input)))
        d = 1;
    else if (!(strcmp("-e", input)))
        e = 1;
    else if (!(strcmp("-f", input)))
        f = 1;
    else if(!first){
        // this input is your search thing
        first = 1;
        strcpy(search, input);
    }
    else if(first == 1){
        if (!(strcmp("~", input)))
            strcpy(path_seek, homedir);
        else if (!(strcmp(".", input)) || input == NULL)
            strcpy(path_seek, curdir);
        else if(!(strcmp("-", input))){
            strcpy(path_seek, predir);
            if(!strcmp("-1", predir)){
                printf("No history of command found\n");
                ok_seek = 0;
                return;
            }
        }
        else{
            if(input[0] == '/'){
                struct stat check;
                stat(input, &check);
                if(!S_ISREG(check.st_mode))
                    strcpy(path_seek, input);
                else{
                    printf("No such directory found\n");
                    ok_seek = 0;
                    return;
                }
                return;
            }
            char here[N];
            strcpy(here, curdir);
            strcat(here, "/");
            strcat(here, input);
            struct stat check;
            stat(here, &check);
            if(!S_ISREG(check.st_mode))
                strcpy(path_seek, here);
            else{
                printf("No such directory found\n");
                ok_seek = 0;
                return;
            }
        }
    }
}

void print_colored_seek(const char *str, const char *color_code) {
    printf("%s%s%s\n", color_code, str, "\033[0m");
}

void remove_extension(char *filename) {
    char *dot = strrchr(filename, '.'); // Find the last occurrence of '.'
    if (dot != NULL) {
        *dot = '\0'; // Truncate the string at the dot
    }
}

void remove_prefix(char *string, const char *prefix) {
    size_t prefix_len = strlen(prefix);
    
    if (strncmp(string, prefix, prefix_len) == 0) {
        memmove(string, string + prefix_len, strlen(string) - prefix_len + 1);
    }
}

void go(const char* cur){
    DIR *here;
    struct dirent *where;
    struct stat state;
    here = opendir(cur);
    if(here == NULL)
        return;
    while ((where = readdir(here)) != NULL) {
        char where_path[N];
        strcpy(where_path, cur);
        strcat(where_path, "/");
        strcat(where_path, where->d_name);

        if (stat(where_path, &state) == 0) {
            char* temp = (char*) malloc(N);
            strcpy(temp, where->d_name);
            remove_extension(temp);
            if (strcmp(temp, search) == 0){
                if (S_ISDIR(state.st_mode)){
                    if(!f){
                        strcpy(temp, where_path);
                        remove_prefix(temp, curdir);
                        char* tempp = (char*) malloc(N);
                        strcpy(tempp, ".");
                        strcat(tempp, temp);
                        print_colored_seek(tempp, "\033[34m"); // Blue for directories
                        strcpy(store, where_path);
                        directory = 1;
                        ++tot;
                    }
                }
                else if (S_ISREG(state.st_mode) && (state.st_mode & S_IXUSR)){
                    // do nothing for executables
                }
                else{
                    if(!d){
                        strcpy(temp, where_path);
                        remove_prefix(temp, curdir);
                        char* tempp = (char*) malloc(N);
                        strcpy(tempp, ".");
                        strcat(tempp, temp);
                        print_colored_seek(tempp, "\033[32m"); // Green for files
                        strcpy(store, where_path);
                        ++tot;
                        directory = 0;
                    }
                }
            }
            if (S_ISDIR(state.st_mode) && strcmp(where->d_name, ".") != 0 && strcmp(where->d_name, "..") != 0) 
                go(where_path);
        }
    }
    closedir(here);
}

void handle_cases_seek(){
    if(d && f){
        printf("Invalid  flags!\n");
        return;
    }
    go(path_seek);
    if(tot == 0){
        printf("No match found!\n");
        return;
    }
    if(e){
        // open the file / directory
        if(tot == 1){
            if(!directory){
                // open only if count of files found is 1
                if(!access(store, R_OK)){
                    FILE *fp = fopen(store, "r");
                    if(fp == NULL){
                        printf("Couldn't open file\n");
                        return;
                    }
                    size_t len = 0;
                    char* line = NULL;
                    while(getline(&line, &len, fp) != -1)
                        printf("%s", line);
                    printf("\n");
                    fclose(fp);
                }
                else{
                    printf("Missing permissions for task!\n");
                    return;
                }
            }
            else if(directory){
                // open only if count of directories found is 1
                if (access(store, R_OK) == -1 && access(store, W_OK) == -1){
                    printf("Missing permissions for task!\n");
                    return;
                }
                char input[N];
                strcat(input, "warp ");
                strcat(input, store);
                warp(input, 0);
            }
        }
    }
}

void seek(char* input){
    init_seek();
    input[strcspn(input, "\n")] = 0;
    char* temp = (char*) malloc(4096);
    strcpy(temp, input);
    char* here;
    int cnt = 1;
    strcpy(path_seek, "-1");
    strcpy(search, "-1");
    here = strtok_r(temp, " ", &temp);
    while(here = strtok_r(temp, " ", &temp)){
        dnc_seek(here);
        ++cnt;
    }
    if(!strcmp("-1", path_seek))
        strcpy(path_seek, curdir);
    if(!ok_seek)
        return;
    if(!first){
        printf("No search file given\n");
        return;
    }
    handle_cases_seek();
}