#ifndef OPTS_H
#define OPTS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trajetInfo {
int id;
int etap;
float tot;
float max;
float min;
float moy;
}inf;

typedef struct avlNode {
float key;
struct trajetInfo traj;
int height;
struct avlNode *fg;
struct avlNode *fd;
}node;



#endif
