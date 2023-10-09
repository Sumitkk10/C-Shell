#include "headers.h"

void proclore(char* input){
    int pid = getpid();
    char* temp = input;
    char* another = input;
    temp = strtok(another, " ");
    temp = strtok(NULL, " ");
    if(temp != NULL){
        // mencaing there is a proces given
        temp[strlen(temp) + 1] = '\0';
        pid = atoi(temp);
    }
    char here[N * 10];
    snprintf(here, sizeof(here), "/proc/%d/status", pid);
    printf("pid : %d\n", pid);
    char* dup = strtok(here, "\n");
    FILE *f = fopen(here, "r");
    char* line = NULL;
    size_t length = 0;
    int index = 0;
    while((getline(&line, &length, f)) != -1){
        // readin the file line by line
        
        if(index == 2){
            int take = getpgid(pid);
            line[strcspn(line, "\n")] = 0;
            if(take == pid) // foreground process
                printf("process status : %c+\n", line[7]);
            else // background process
                printf("process status : %c\n", line[7]);
        }
        else if(index == 17){
            char* proc = line;
            char* pro = strtok(proc, ":");
            pro = strtok(NULL, ":");
            pro[strcspn(pro, "\n")] = 0;
            printf("Process Group : %d\n", getpgid(pid));
            char* real = (char*) malloc(N);
            int str = 0;
            while(str < strlen(pro) && (pro[str] == ' ' || pro[str] == '\t')){
                str++;
            }
            int k = 0;
            for(int i = str; i < strlen(pro) - 2; ++i)
                real[k++] = pro[i];
            real[k] = '\0';
            printf("Virtual memory : %s\n", real);
            break;
        }
        dup = strtok(NULL, "\n");
        ++index;
    }
    fclose(f);
    sprintf(here, "/proc/%d/exe", pid);
    char path[N];
    int where;
    while((where = readlink(here, path, N)) < 0){
        printf("executable path : path does not exist.\n");
        return;
    }
    path[where] = '\0';
    char res[N];
    res[0] = '~';
    bool ok = 1;
    for(int i = 0; i < strlen(homedir); ++i){
        if(homedir[i] != path[i]){
            ok = 0;
            break;
        }
    }
    if(ok){
        for(int i = strlen(homedir); i < strlen(path); ++i)
            res[i - strlen(homedir) + 1] = path[i];
        res[strlen(path) - strlen(homedir) + 1] = '\0';
    }
    else
        strcpy(res, path);
    printf("exectuable path : %s\n", res);
}