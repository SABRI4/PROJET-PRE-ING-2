#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 350000 // ??
#define IGN1 0

/*
ajouter les free
mettre une condition de modulo sur les occurences impaires ?
rajoiter macro pour ignorer les occ == 1 ?
*/

//erreurs :
// 2 villes pas presente dans le top 10
// occ incorrecte pour certaines villes

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
//int tId[2000];
//int idxTID;
    char dataTab[MAXSIZE][60];
    int occdTab[MAXSIZE];
    int idx;
}node;


inf *newInfo(char *ville, int occd) {
    inf *new = malloc(sizeof(inf));
    if(new != NULL) {
	new->ville = malloc(60*sizeof(char));
        strcpy(new->ville, ville);
        new->occd = occd;
        new->occ = 1;
        return new;
    }
    else {
        printf("newInfo is NULL for %s\n", ville);
        exit(1);
    }
}
node *newNode(inf *city) {
    node *new = malloc(sizeof(node));
    if(new != NULL) {
        new->city = *city;
        new->height = -1;
        new->fd = NULL;
        new->fg = NULL;
        /*for (int i = 0; i < MAXSIZE; i++) {
            new->dataTab[i][0] = '\0';  // Set the first character to null terminator, indicating an empty string
            new->occdTab[i] = 0;

        }*/
        new->idx = 0;// 0 ou 1?
        return new;
    }
        else {
        printf("newNode is NULL for %s\n", city->ville);
        exit(1);
    }
}

int stringComp(char *s1, char*s2) {

        return strcmp(s1, s2);
}

int max(int a, int b) {
if (a > b) {
return a;
}
else {
return b;
} 
}

int getH(node *avl) {
if(avl == NULL) {
return 0;
}
return avl->height;
}

int getB(node *avl) {
if(avl == NULL) {
return 0;
}
return getH(avl->fg) - getH(avl->fd);
}

node *RRotate(node *y) {
node *x = y->fg;
node *T2 = x->fd;

y->fd = y;
y->fg = T2;

y->height = 1 + max(getH(y->fg), getH(y->fd));
x->height = 1 + max(getH(x->fg), getH(x->fd));

return x;
}

node *LRotate(node *x) {
node *y = x->fd;
node *T2 = y->fg;

y->fg = x;
x->fd = T2;

x->height = 1 + max(getH(x->fg), getH(x->fd));
y->height = 1 + max(getH(y->fg), getH(y->fd));

return y;
}

node *insert(node *avl, inf *city) {
    if(avl == NULL) {
        return newNode(city);
    }
    
    int key = stringComp(city->ville, avl->city.ville);
    


    if(key > 0) {
        avl->fd = insert(avl->fd, city);
    }
    else if(key < 0) {
        avl->fg = insert(avl->fg, city);
    }
    else if(key == 0) {
//printf("INCREASED OCC FOR  %s OLD : %d NEW : %d\n", avl->city.ville, avl->city.occ, avl->city.occ + 1);
        avl->city.occ++;
        avl->city.occd += city->occd;
	free(city->ville);
        free(city);
    }
    
       int balance = getB(avl);
if(avl->fg || avl->fd == NULL) {
return avl;
}

if(avl->fg != NULL) {
//ici rotations
if (balance > 1 && key < avl->fg->city.occ) {
return RRotate(avl);
}
}

if(avl->fd != NULL) {
if (balance < -1 && key > avl->fd->city.occ){
return LRotate(avl);
}
}
if(avl->fg != NULL) {
if (balance > 1 && key > avl->fg->city.occ) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}
}

if(avl->fd !=  NULL) {
if (balance < -1 && key < avl->fd->city.occ) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}
}
if(avl->fg != NULL) {
if (balance > 1 && key > avl->fg->city.occ) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}
}

if(avl->fd != NULL) {
if (balance < -1 && key < avl->fd->city.occ) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}
}


return avl;
}

node * proCsv(char *csv, int row, node *avl) {
FILE *file = fopen(csv, "r");
if( file == NULL) {printf("file NULL\n"); exit(1);}
char buff[256];
int col;
int ct = 0 ; // 1 pour eviter town a town b
int step;
int tid;
int fline = 1;

/*void adjust(node *avl) {
    if(avl == NULL) {
        return;
    }
    
    adjust(avl->fg);

        int occ = avl->city.occ;
        int occd = avl->city.occd;

        if(avl->city.occ > 1) {
    int nocc = (occ - occd)/2;
    nocc += occd;
    avl->city.occ = nocc;        
    }
    adjust(avl->fd);
    
}*/

while(fgets(buff, sizeof(buff), file) != NULL && ct < row) {
char *token = strtok(buff, ";");
col = 1;
//printf("-------------------------------------ROW %d----------------------\n", ct);// A DELETE
if(fline) {
printf("skipping fline\n");
fline = 0;
ct++;
continue;
}

while(token != NULL && col <= 6) {
//printf("inside while 1 || COL = %d\n", col);
if(col == 2) {
step = atoi(token);
}
else if(col == 3 || col == 4) {
if(step == 1 && col == 3) {//logique d'ajt en cas de ville1 == épart
inf *city = newInfo(token, 1);
//printf("%s occ at creation : %d\n", city->ville, city->occ);
avl = insert(avl, city);
}
else {
inf *city = newInfo(token, 0);
//printf("%s occ at creation : %d\n", city->ville, city->occ);
avl = insert(avl, city);
}
}
token = strtok(NULL, ";");
col++; 
}
ct++;
}
printf("fclose file\n");
fclose(file);
return avl;
}

node *insert2(node *avl, inf *city) {
    if(avl == NULL) { 
        return newNode(city);
    }
    
    int key = city->occ;
    
    if(key > avl->city.occ) {
        avl->fd = insert2(avl->fd, city);
    }
    else if(key < avl->city.occ) {
        avl->fg = insert2(avl->fg, city);
    }
    else if(key == avl->city.occ) {
        strcpy(avl->dataTab[avl->idx], city->ville);
        avl->occdTab[avl->idx] = city->occd;
        avl->idx++;
    }
    
           int balance = getB(avl);

//ici rotations
if (balance > 1 && key < avl->fg->city.occ) {
return RRotate(avl);
}

if (balance < -1 && key > avl->fd->city.occ){
return LRotate(avl);
}

if (balance > 1 && key > avl->fg->city.occ) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}

if (balance < -1 && key < avl->fd->city.occ) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}
if (balance > 1 && key > avl->fg->city.occ) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}

if (balance < -1 && key < avl->fd->city.occ) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}

return avl;
}

node *parseAvl(node *avl1, node *avl2) {
    if(avl1 == NULL) {
        return NULL;
    }
    parseAvl(avl1->fg, avl2);
    if((avl1->city.occ > 1 &&  IGN1) || IGN1 == 0) {
    avl2 = insert2(avl2, &avl1->city);
}
    parseAvl(avl1->fd, avl2);
    
    return avl2;
}

void freeAvl1(node *avl) {
if(avl == NULL) {
return;
}
freeAvl1(avl->fg);
freeAvl1(avl->fd);

free(avl);

}

void parcours(node *avl, int counter) {
    
    parcours(avl->fd, counter);
    
    //logic
    counter++;
    
    parcours(avl->fg, counter);
}

void testParcours(node *avl) {
        if(avl == NULL) {
return;
}    
    testParcours(avl->fg);
    
    printf("MAIN : VILLE %s | OCC %d | OCCD %d | IDX %d\n", avl->city.ville, avl->city.occ, avl->city.occd, avl->idx);
    for(int i = 0; i < avl->idx; i++) {
        printf("    IDX %d :    VILLE %s | OCC %d | OCCD %d\n", i, avl->dataTab[i], avl->city.occ, avl->occdTab[i]);
    }
    
    testParcours(avl->fd);
}

void adjust(node *avl) {
    if(avl == NULL) {
        return;
    }
    
    adjust(avl->fg);

        int occ = avl->city.occ;
        int occd = avl->city.occd;

        if(occ > 1) {
        printf("ADJUSTING %s \n", avl->city.ville);
    int nocc = (occ - occd)/2;
    nocc += occd;
if(nocc > 0) {
    avl->city.occ = nocc;        
}
else {
printf("nocc ! > 0\n");
exit(1);
}    
}
    adjust(avl->fd);
    
} 

int main(int argc, char *argv[])
{   
    int row = 0;
    char csv[50];
    if(argc < 2) {
        printf("usage : ./file <rows> <csv file>\n");
    }
    else {
        row = atoi(argv[1]);
        strcpy(csv, argv[2]);
    }
    node *avl1 = NULL;
    node *avl2 = NULL;
   printf("launch proCsv\n");
    avl1 = proCsv(csv, row, avl1);
    printf("launch adjustement\n");
    adjust(avl1);
    printf("launch parseAvl\n");
    avl2 = parseAvl(avl1, avl2);
freeAvl1(avl1);    
if(avl2 == NULL){printf("avl2 = NULL\n");exit(1);}
        printf("launch test parcours\n");
    testParcours(avl2);
    //printf("launch Parcours\n");
    //parcours(avl2, 0, -1);
}
