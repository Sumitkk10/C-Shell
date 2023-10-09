#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define N 4096

extern char curdir[N], sysname[N], homedir[N], predir[N];

typedef struct bgpro{
    int pid;
    char command[N];
    char status;
    int proid;
} bgpro;

extern bgpro processes[N];
extern int totalpro;

#include "call.h"
#include "warp.h"
#include "prompt.h"
#include "proclore.h"
#include "pastevents.h"
#include "peek.h"
#include "seek.h"
#include "background.h"

#endif