#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MACRO_LOWSAMPLE 0 // en cas de petit nb de row le max = le min car pas de plusieurs etap
#define MACRO_PRINTROW 0 //permet de print les ----ROW---- ou non

/*
free traj / avl / avl2
commentaires
*/

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

int counter;

node *newNode(inf *dist) {
node *new = malloc(sizeof(node));
if(new != NULL) {
new->traj = *dist;
new->height =  1;
new->fg = NULL;
new->fd = NULL;
return new;
}
}
inf *newInfo(int key, float dist) {
inf *new = malloc(sizeof(inf));
if(new != NULL) {
new->id = key;
new->etap = 1;
new->moy = dist;
new->tot = 0;
new->min = dist;
new->max = dist;
return new;
}
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

node *insert2(node *avl, inf *newInf) {
//printf("INSERT2 CALL WITH KEY %d | DIST %f\n", newInf->id, newInf->max);
if(avl == NULL){
return newNode(newInf);
}
int key = newInf->id;

if(key > avl->traj.id) {
avl->fd = insert2(avl->fd, newInf);
}
else if(key < avl->traj.id) {
avl->fg = insert2(avl->fg, newInf);
}
else if(key == avl->traj.id){ 
//printf("EGALITE DE TRAJETID  %d \n", key);
if(newInf->max > avl->traj.max) {avl->traj.max = newInf->max;}
if(newInf->min < avl->traj.min) {avl->traj.min = newInf->min;}
//printf("ADDED %f to MOY %f\n", newInf->min, avl->traj.moy);
avl->traj.moy += newInf->min;
avl->traj.etap++;
}

int balance = getB(avl);

//ici rotations
if (balance > 1 && key < avl->fg->traj.id) {
return RRotate(avl);
}

if (balance < -1 && key > avl->fd->traj.id){
return LRotate(avl);
}

if (balance > 1 && key > avl->fg->traj.id) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}
if (balance < -1 && key < avl->fd->traj.id) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}
if (balance > 1 && key > avl->fg->traj.id) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}

if (balance < -1 && key < avl->fd->traj.id) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}

return avl;

}

node * proCsv(node *avl, char *csv, int row) {  // pro csv a adapter a la logique de -s
FILE *file = fopen(csv, "r");
if( file == NULL) {printf("file NULL\n"); exit(1);}

char buff[256];
int col;
int ct = 0 ; // 1 pour eviter town a town b
int trajet;
int fline = 1;

while(fgets(buff, sizeof(buff), file) != NULL && ct < row) {
char *token = strtok(buff, ";");
col = 1;
if(MACRO_PRINTROW){
printf("-------------------------------------ROW %d----------------------\n", ct);// A DELETE
}
if(fline) {
fline = 0;
ct++;
continue;
}

while(token != NULL && col <= 6) {
//printf("inside while 1 || COL = %d\n", col);
if(col == 1) {
trajet = atoi(token);
}
else if(col == 5) {
//printf("proCsv : call hashInsert with %d & %f\n", trajet, atof(token));
inf *newInf = newInfo(trajet, atof(token));
avl = insert2(avl, newInf);
}
token = strtok(NULL, ";");
col++; 
}
ct++;
}
fclose(file);
return avl;
}
node *insert(node *avl, inf *dist) {
//printf("INSERT CALL WITH KEY %f | MAX %f(already good max)\n", dist->tot, dist->max);
if(avl == NULL){
return newNode(dist);
}
float key = dist->tot;
//printf("CITY KEY %d\n", key);
//printf("AVLINSERT : %d | %f\n", dist->id, key);
if(key > avl->traj.tot) {
avl->fd = insert(avl->fd, dist);
}
else if(key < avl->traj.tot) {
avl->fg = insert(avl->fg, dist);
}
else if(key == avl->traj.tot){ //meme nb d'occ mais ville forcement differentes car doublons rassemlbés dans htable.
//printf("DEUX DISTANCES TOTALES EGALES (CAS PARTICULIER) = %f\n", key); // cas impossible /tres rare ?
}

int balance = getB(avl);

//ici rotations
if (balance > 1 && key < avl->fg->traj.tot) {
return RRotate(avl);
}

if (balance < -1 && key > avl->fd->traj.tot){
return LRotate(avl);
}

if (balance > 1 && key > avl->fg->traj.tot) {
avl->fg = LRotate(avl->fg);
}

if (balance < -1 && key < avl->fd->traj.tot) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}
if (balance > 1 && key > avl->fg->traj.tot) {
avl->fg = LRotate(avl->fg);
return RRotate(avl);
}

if (balance < -1 && key < avl->fd->traj.tot) {
avl->fd = RRotate(avl->fd);
return LRotate(avl);
}

return avl;

}

node *parseTable(node **avl, node *avl2) {
if(avl2 == NULL) {
return NULL;
}
parseTable(avl, avl2->fg);

//printf("PARSETABLE AVL1 CALL %f\n", avl2->traj.tot);
inf *new = &avl2->traj;
*avl = insert(*avl, new);

parseTable(avl, avl2->fd);

return *avl;
}

node *adjust(node *avl) {
if(avl == NULL) {
return NULL;
}
adjust(avl->fg);

avl->traj.moy = (avl->traj.moy)/avl->traj.etap;
if(MACRO_LOWSAMPLE == 0) {
avl->traj.tot = avl->traj.max - avl->traj.min;
}
else {
avl->traj.tot = avl->traj.max; //pr les petit sample, max = min par def de newInf donc fausse insert1
}
//printf("ADJUSTED MOY TO %f\n", avl->traj.moy);
//printf("ADJUSTED TOT TO %f\n", avl->traj.tot);

adjust(avl->fd);

return avl;
}

void parcours(node *avl) { // inverser l'ordre (affichage du terminal trop petit donc + grand en dernier)
if(avl == NULL) {
return;
}
parcours(avl->fd);

if(counter < 50) {
printf("%d |ID|%d;|TOTAL|%f;|MOYENNE|%f;|ETAPES|%d|MAX|%f;|MIN|%f\n", counter, avl->traj.id, avl->traj.tot, avl->traj.moy, avl->traj.etap, avl->traj.max, avl->traj.min);
//printf("ID %d | TOTAL %f | MOYENNE %f | MAX %f | MIN %f | ETAPE %d | BAL %d\n", avl->traj.id, avl->traj.tot, avl->traj.moy, avl->traj.max, avl->traj.min, avl->traj.etap, getB(avl));
counter++;
}

parcours(avl->fg);
}

int main(int argc, char *argv[]) {

//clock_t start_time = clock();
int row = 0;
char csv[50];

node *avl = NULL;
node *avl2 = NULL;

if(argc < 2) {
printf("USAGE : ./file <rows> <csv_file>\n");
exit(1);
}
else {
row = atoi(argv[1]);
strcpy(csv, argv[2]);
}
avl2 = proCsv(avl2, csv, row);
avl2 = adjust(avl2); // crée les moyennes + crée les totaux
avl = parseTable(&avl, avl2);
//clock_t time1 = clock();
counter = 0;
parcours(avl);
//clock_t time2 = clock();
//double elap1 = ((double)(time2 - start_time))/CLOCKS_PER_SEC;
//double elap2 = ((double)(time1 - start_time))/CLOCKS_PER_SEC;
//printf("EXECTIME : TOTAL = %f\nEXECTIME : REAL = %f\n", elap1, elap2);
}
