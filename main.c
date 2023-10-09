#include "headers.h"

void remove_extra_spaces(char *str) {
    int len = strlen(str);
    int i, j;
    bool space_found = false;
    int new_length = 0;

    for (i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\t') {
            str[new_length++] = str[i];
            space_found = false;
        } else if (i > 0 && !space_found) {
            str[new_length++] = ' ';
            space_found = true;
        }
    }

    // Handle trailing space
    if (new_length > 0 && str[new_length - 1] == ' ') {
        str[new_length - 1] = '\0';
    } else {
        str[new_length] = '\0';
    }
}

int main()
{
    getcwd(homedir, N);
    strcpy(curdir, homedir);
    strcpy(predir, "-1");
    char* out = (char*) malloc(N);
    strcpy(out, "-1");
    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(out);
        strcpy(out, "-1");
        char* inp = (char*) malloc(N);
        fgets(inp, N, stdin);
        char* input = (char*) malloc(N);
        int cr = 0;
        for(int i = 0; i < strlen(inp); ++i){
            input[cr++] = inp[i];
            if(inp[i] == '&')
                input[cr++] = ';';
        }
        input[cr] = '\0';
        remove_extra_spaces(input);
        checkprocess();
        if(strstr(input, "pastevents") == NULL){
            add(input);
        }
        // tokenize based on ; and then based on &
        char* temp = (char*) malloc(N);
        strcpy(temp, input);
        char* here = strtok_r(temp, ";", &temp);
        // printf("%s\n", here);
        int cnt = 0;
        while(here != NULL){
            if(!strcmp(out, "-1"))
                out = call(here);
            else
                call(here);
            here = strtok_r(NULL, ";", &temp);
        }
    }
}
