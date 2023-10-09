#include "headers.h"
bool l, a, ok;
char path[N];

void init(){
    l = 0;
    a = 0;
    ok = 1;
}

void dnc(char* input){ // divide and conquer 
    if (!(strcmp("-al", input)) || !(strcmp("-la", input))){
        a = 1;
        l = 1;
    }
    else if (!(strcmp("-l", input)))
        l = 1;
    else if (!(strcmp("-a", input)))
        a = 1;
    else if (!(strcmp("~", input)))
        strcpy(path, homedir);
    else if (!(strcmp(".", input)) || input == NULL)
        strcpy(path, curdir);
    else if(!(strcmp("-", input))){
        strcpy(path, predir);
        if(!strcmp("-1", predir)){
            printf("No history of command found\n");
            ok = 0;
            return;
        }
    }
    else{
        if(input[0] == '/'){
            struct stat check;
            stat(input, &check);
            if(!S_ISREG(check.st_mode))
                strcpy(path, input);
            else{
                printf("No such directory found\n");
                ok = 0;
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
            strcpy(path, here);
        else{
            printf("No such directory found\n");
            ok = 0;
            return;
        }
    }
}

int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_colored(const char *str, const char *color_code) {
    printf("%s%s%s\n", color_code, str, "\033[0m");
}

void perms(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void handle_cases(){
    DIR *cur;
    struct dirent *entry;
    struct stat here;
    char *entries[N]; 
    cur = opendir(path);
    if(cur == NULL){
        printf("couldn't open the directory\n");
        return ;
    }
    if(!l && !a){
        // just print the contents of the directory path;
        int cnt = 0;
        while ((entry = readdir(cur)) != NULL) {
            if (entry->d_name[0] != '.') {  // Exclude hidden files and directories
                entries[cnt] = strdup(entry->d_name);
                cnt++;
            }
        }
        closedir(cur);
        qsort(entries, cnt, sizeof(char *), cmp);
        for (int i = 0; i < cnt; i++) {
            char tot_info[N];
            strcpy(tot_info, path);
            strcat(tot_info, "/");
            strcat(tot_info, entries[i]);
            stat(tot_info, &here);
            if (S_ISDIR(here.st_mode))
                print_colored(entries[i], "\033[34m"); // Blue for directories
            else if (here.st_mode & S_IXUSR)
                print_colored(entries[i], "\033[32m"); // Green for executables
            else 
                printf("%s\n", entries[i]); // White for other files
            free(entries[i]);
        }
    }
    else if(l && !a){
        // display extra information
        int cnt = 0;
        while ((entry = readdir(cur)) != NULL) {
            if (entry->d_name[0] != '.') {  // Exclude hidden files and directories
                entries[cnt] = strdup(entry->d_name);
                cnt++;
            }
        }
        closedir(cur);
        qsort(entries, cnt, sizeof(char *), cmp);
        for (int i = 0; i < cnt; i++) {
            char tot_info[N];
            strcpy(tot_info, path);
            strcat(tot_info, "/");
            strcat(tot_info, entries[i]);
            if(stat(tot_info, &here) == 0){
                perms(here.st_mode);
                printf(" %lu ", here.st_nlink);
                struct passwd *pwd = getpwuid(here.st_uid);
                struct group *grp = getgrgid(here.st_gid);
                printf("%s %s ", pwd ? pwd->pw_name : "", grp ? grp->gr_name : "");
                printf("%lld ", (long long) here.st_size);
                char time[N];
                strftime(time, sizeof(time), "%b %d %H:%M", localtime(&here.st_mtime));
                printf("%s ", time);
                if (S_ISDIR(here.st_mode))
                    print_colored(entries[i], "\033[34m"); // Blue for directories
                else if (here.st_mode & S_IXUSR)
                    print_colored(entries[i], "\033[32m"); // Green for executables
                else 
                    printf("%s\n", entries[i]); // White for other files
            }
            free(entries[i]); // Free dynamically allocated memory
        }
    }
    else if(a && !l){
        // display all files
        int cnt = 0;
        while ((entry = readdir(cur)) != NULL) {
            entries[cnt] = strdup(entry->d_name);
            cnt++;
        }
        closedir(cur);
        qsort(entries, cnt, sizeof(char *), cmp);
        for (int i = 0; i < cnt; i++) {
            if (S_ISDIR(here.st_mode))
                print_colored(entries[i], "\033[34m"); // Blue for directories
            else if (here.st_mode & S_IXUSR)
                print_colored(entries[i], "\033[32m"); // Green for executables
            else 
                printf("%s\n", entries[i]); // White for other files
            free(entries[i]); // Free dynamically allocated memory
        }
    }
    else{
        // display extra information of all files
        int cnt = 0;
        while ((entry = readdir(cur)) != NULL) {
            entries[cnt] = strdup(entry->d_name);
            cnt++;
        }
        closedir(cur);
        qsort(entries, cnt, sizeof(char *), cmp);
        for (int i = 0; i < cnt; i++) {
            char tot_info[N];
            strcpy(tot_info, path);
            strcat(tot_info, "/");
            strcat(tot_info, entries[i]);
            if(stat(tot_info, &here) == 0){
                perms(here.st_mode);
                printf(" %lu ", here.st_nlink);
                struct passwd *pwd = getpwuid(here.st_uid);
                struct group *grp = getgrgid(here.st_gid);
                printf("%s %s ", pwd ? pwd->pw_name : "", grp ? grp->gr_name : "");
                printf("%lld ", (long long) here.st_size);
                char time[N];
                strftime(time, sizeof(time), "%b %d %H:%M", localtime(&here.st_mtime));
                printf("%s ", time);
                if (S_ISDIR(here.st_mode))
                    print_colored(entries[i], "\033[34m"); // Blue for directories
                else if (here.st_mode & S_IXUSR)
                    print_colored(entries[i], "\033[32m"); // Green for executables
                else 
                    printf("%s\n", entries[i]); // White for other files
            }
            free(entries[i]); // Free dynamically allocated memory
        }
    }
}

void peek(char* input){
    init();
    input[strcspn(input, "\n")] = 0;
    char* temp = (char*) malloc(4096);
    strcpy(temp, input);
    char* here;
    int cnt = 1;
    strcpy(path, "-1");
    here = strtok_r(temp, " ", &temp);
    while(here = strtok_r(temp, " ", &temp)){
        dnc(here);
        ++cnt;
    }
    if(!strcmp("-1", path))
        strcpy(path, curdir);
    if(!ok)
        return;
    handle_cases();
}