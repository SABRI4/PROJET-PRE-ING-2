#include "opts.h"


#define MACRO_LOWSAMPLE 0 // en cas de petit nb de row le max = le min car pas de plusieurs etap


int counter;


//fonction qui retourne un noeud de l'avl
node *newNode(inf *dist) {
node *new = malloc(sizeof(node));
if(new != NULL) {
new->traj = *dist;
new->height =  1;
new->fg = NULL;
new->fd = NULL;
return new;
}
  return NULL;
}

//fonction qui retourne une structure contenant les stats nécessaire au traitement de l'option s
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
  return NULL;
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


//fonction d'insertion dans le premier avl : trie les trajets arbitrairement par leur ID, si deux ID sont identiques, on additionne les valeurs
node *insert2(node *avl, inf *newInf) {
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
if(newInf->max > avl->traj.max) {avl->traj.max = newInf->max;}
if(newInf->min < avl->traj.min) {avl->traj.min = newInf->min;}
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


//fonction de lécture du fichier CSV
node * proCsv(node *avl, char *csv, int row) { 
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
if(fline) {
fline = 0;
ct++;
continue;
}

while(token != NULL && col <= 6) {
if(col == 1) {
trajet = atoi(token);
}
else if(col == 5) {
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

//insértion dans le second avl en fonction de "tot" qui est max - min (logique demandée dans la déscription du projet)
node *insert(node *avl, inf *dist) {
if(avl == NULL){
return newNode(dist);
}
float key = dist->tot;
if(key > avl->traj.tot) {
avl->fd = insert(avl->fd, dist);
}
else if(key < avl->traj.tot) {
avl->fg = insert(avl->fg, dist);
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

//fonction qui parcours le premier avl et insert la structure "traj" (trajet) dans le second avl
node *parseTable(node **avl, node *avl2) {
if(avl2 == NULL) {
return NULL;
}
parseTable(avl, avl2->fg);

inf *new = &avl2->traj;
*avl = insert(*avl, new);
free(avl2);

parseTable(avl, avl2->fd);

return *avl;
}

//fonction qui permet de calculer la moyenne et le total, car ces deux valeurs ne sont calculable qu'après avoir obtenu l'ensemble des données
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
avl->traj.tot = avl->traj.max; 
}

adjust(avl->fd);

return avl;
}

//print les résultats selon la quantité demandée et au format adapté à GNUPLOT
void parcours(node *avl) { 
if(avl == NULL) {
return;
}
parcours(avl->fd);

if(counter < 50) {
printf("%d |ID|%d;|TOTAL|%f;|MOYENNE|%f;|ETAPES|%d|MAX|%f;|MIN|%f\n", counter, avl->traj.id, avl->traj.tot, avl->traj.moy, avl->traj.etap, avl->traj.max, avl->traj.min);
free(&avl->traj);
counter++;
}

parcours(avl->fg);
}

void freeAVL(node *avl) {
if(avl == NULL) {
  return NULL;

  freeAVL(avl->fg);
  freeAVL(avl->fd);

  free(avl->traj);
  free(avl);
  
}

int main(int argc, char *argv[]) {

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
counter = 0;
parcours(avl);
freeAVL(avl);
}
