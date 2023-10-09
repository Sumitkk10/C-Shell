#include "headers.h"

char curdir[N], sysname[N], homedir[N], predir[N];

void prompt(char* print) {
    getcwd(curdir, N); // returns the current directory on shell
    gethostname(sysname, N); 
    char* username = getlogin();
    char res[N];
    res[0] = '~';
    bool ok = 1;
    for(int i = 0; i < strlen(homedir); ++i){
        if(homedir[i] != curdir[i]){
            ok = 0;
            break;
        }
    }
    if(ok){
        // it is somewhere inside the original start directory
        for(int i = strlen(homedir); i < strlen(curdir); ++i){
            res[i - strlen(homedir) + 1] = curdir[i];
        }
        res[strlen(curdir) - strlen(homedir) + 1] = '\0';
    }
    else{
        // it is outside (meaning parent side) of the original start drectory
        strcpy(res, curdir);
    }
    if(!strcmp("-1", print))
        printf("<%s@%s:%s>", username, sysname, res);
    else
        printf("<%s@%s:%s %s>", username, sysname, res, print);
}
