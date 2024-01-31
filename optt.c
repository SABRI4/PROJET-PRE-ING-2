#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 350000 // ??
#define IGN1 1

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
    int TID[2000];
    int idxTID;
    char dataTab[MAXSIZE][60];
    int occdTab[MAXSIZE];
    int idx;
}node;

int counter;

inf *newInfo(char *ville, int occd) {
    inf *new = malloc(sizeof(inf));
    if(new != NULL) {
        new->ville = malloc(60*sizeof(char));
        strcpy(new->ville, ville);
        new->occd = occd;
        new->occ = 0;
        return new;
    }
    else {
        printf("newInfo is NULL for %s\n", ville);
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

    // Perform rotation
    x->fd = y;
    y->fg = T2;

    // Update heights
    y->height = 1 + max(getH(y->fg), getH(y->fd));
    x->height = 1 + max(getH(x->fg), getH(x->fd));

    return x;  // New root
}

node *LRotate(node *x) {
    node *y = x->fd;
    node *T2 = y->fg;

    // Perform rotation
    y->fg = x;
    x->fd = T2;

    // Update heights
    x->height = 1 + max(getH(x->fg), getH(x->fd));
    y->height = 1 + max(getH(y->fg), getH(y->fd));

    return y;  // New root
}
int compID(int tab[2000], int TID, int size) {
    for(int i = 0; i < size; i++) {
        if(tab[i] == TID) {
            return 0;
        }
    }
    return TID;
}

node *insert(node *avl, inf *city, int TID) {
    if(avl == NULL) {
        return newNode(city);
    }
    
    int key = stringComp(city->ville, avl->city.ville);
    

    if(key > 0) {
        avl->fd = insert(avl->fd, city, TID);
    }
    else if(key < 0) {
        avl->fg = insert(avl->fg, city, TID);
    }
    else if(key == 0) {
//printf("INCREASED OCC FOR  %s OLD : %d NEW : %d\n", avl->city.ville, avl->city.occ, avl->city.occ + 1);

        if(compID(avl->TID, TID, avl->idxTID) == 0) { // une seule fois une ville par trajet
        free(city->ville);
        free(city);
        return avl;           
        }
        else {
        avl->TID[avl->idxTID] = TID;
            avl->idxTID++;

        avl->city.occ++;
        avl->city.occd += city->occd;
        free(city->ville);
        free(city);
}
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
//printf("skipping fline\n");
fline = 0;
ct++;
continue;
}

while(token != NULL && col <= 6) {
//printf("inside while 1 || COL = %d\n", col);
if(col == 1) {
    tid = atoi(token);
}
if(col == 2) {
step = atoi(token);
}
else if(col == 3 || col == 4) {
if(step == 1 && col == 3) {//logique d'ajt en cas de ville1 == épart
inf *city = newInfo(token, 1);
//printf("%s occ at creation : %d\n", city->ville, city->occ);
avl = insert(avl, city, tid);
}
else {
inf *city = newInfo(token, 0);
//printf("%s occ at creation : %d\n", city->ville, city->occ);
avl = insert(avl, city, tid);
}
}
token = strtok(NULL, ";");
col++; 
}
ct++;
}
//printf("fclose file\n");
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

node *parseAvl(node *avl1, node **avl2) {
    if(avl1 == NULL) {
        return NULL;
    }
   parseAvl(avl1->fg, avl2);
   /* if(strcmp(avl1->city.ville, "ANDILLY") == 0 || (strcmp(avl1->city.ville, "BEAULIEU") == 0)) {
printf("%s -> %d | %d\n", avl1->city.ville, avl1->city.occ, avl1->city.occd);
exit(1);
}*/
        if((avl1->city.occ > 1 &&  IGN1) || IGN1 == 0) {

*avl2 = insert2(*avl2, &avl1->city);
}
   parseAvl(avl1->fd, avl2);
    
    return *avl2;
}

void freeAvl1(node *avl) {
if(avl == NULL) {
return;
}
freeAvl1(avl->fg);
freeAvl1(avl->fd);

free(avl);

}

void parcours(node *avl, int a1[], int a2[], char a3[][60]){
if(avl == NULL) {
return;
}
    parcours(avl->fd, a1, a2, a3);    
        if(counter < 10) {
        strcpy(a3[counter], avl->city.ville);
        a1[counter] = avl->city.occ;
        a2[counter] = avl->city.occd;
        counter++; }    
        for(int i = 0; i < avl->idx; i++) {
        if(counter < 10) {
        strcpy(a3[counter], avl->dataTab[i]);
        a1[counter] = avl->city.occ;
        a2[counter] = avl->occdTab[i];
        counter++;
        }
        }    
    
    parcours(avl->fg, a1 , a2, a3);
}
void swapTAB(char a3[][60], int a1[], int a2[], int j, int j2) {

    char temp[60];
    strcpy(temp, a3[j]);
    strcpy(a3[j], a3[j2]);
    strcpy(a3[j2], temp);

    int temp1 = a1[j];
    a1[j] = a1[j2];
    a1[j2] = temp1;

    int temp2 = a2[j];
    a2[j] = a2[j2];
    a2[j2] = temp2;

}

void sort(int n, int a1[], int a2[], char a3[][60]) {

    for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
    if (strcmp(a3[j], a3[j + 1]) > 0) {
    swapTAB(a3, a1, a2, j, j + 1);
  }
}
}

}

void print(int n, int a1[], int a2[], char a3[][60]){

for(int i = 0; i < n; i++) {
printf("%d |%s;|OCC|%d;|OCCD| %d\n", i, a3[i], a1[i], a2[i]);

} 

}
int main(int argc, char *argv[])
{   

    int occdTAB[10];
    int occTAB[10];
    char villeTAB[10][60];
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
   //printf("launch proCsv\n");
    avl1 = proCsv(csv, row, avl1);
    //printf("launch adjustement\n");
        //printf("no adjust\n");    
//adjust(avl1);
    //printf("launch parseAvl\n");
//unkt2(avl1);    
avl2 = parseAvl(avl1, &avl2);
freeAvl1(avl1);    
if(avl2 == NULL){printf("avl2 = NULL\n");exit(1);}
//unkt(avl2);       
counter = 0;
parcours(avl2, occTAB, occdTAB, villeTAB);
sort(10, occTAB, occdTAB, villeTAB);
print(10, occTAB, occdTAB, villeTAB);

}
