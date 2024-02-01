#ifndef OPTT_H
#define OPTT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct info {
    char *ville;
    int occ;
    int occd;
}inf;

typedef struct node {
    struct info city;
    int height;
    struct node *fd;
    struct node *fg;
    int TID[2000];
    int idxTID;
    char dataTab[MAXSIZE][60];
    int occdTab[MAXSIZE];
    int idx;
}node;

#endif
