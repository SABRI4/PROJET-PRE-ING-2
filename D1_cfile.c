script c pour l'option d1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bib/gen.h"
#define MAX_ROW 5000000 // fonctionnement strtok ? atoi ? meilleur allocation dynamique ? test sur l'ensemble de>

int compare(int x1, int x2, int compar1[], int compar2[], int rc) {

int r = 1;
printf("compare func entry\n");
for (int i = 0; i < rc; i++) {
printf("comparing entry %d\n", i);
        if (x1 == compar1[i] && x2 != compar2[i]) {
        r = 0;
        printf("entry %d proven to be doubled\n", i);
        i = rc;
        }

}
return r;
}


int main(int argc, char *argv[]) {      // delete all with same routeID, count occurrence of each name, tri algo>

printf("test1 mainstart\n");
printf("ARGV 2 : %s\n", argv[2]);
if(argc < 3) {
printf("pas d'arguments pour input data progc");
exit(1);
}
int lsize = atoi(argv[2]);
char csv_file[50];
//char csv_file1[50];
//strncpy(csv_file, "../", sizeof(csv_file) -1);
//strncpy(csv_file1, "data/data.csv", sizeof(csv_file1) -1);
//strncpy(csv_file1, (const char*)argv[1], sizeof(csv_file1) -1);  // à alloouer malloc ????
//strcat("../", csv_file); erreur  de segfault ici 
//strncat(csv_file, csv_file1, sizeof(csv_file) -strlen(csv_file1) -1);
strncpy(csv_file, argv[1], sizeof(csv_file) -1);
printf("DATA FILE LOCATION : %s\n", csv_file);
printf("DATA FILE SIZE : %d\n", lsize);

snprintf(csv_file, sizeof(csv_file), "../%s", argv[1]);

printf("test2 after snprintf\n");

FILE *file = fopen(csv_file, "r");
printf("after file open\n");
if (file == NULL) { //comment traiter éfficacement les erreur en les retourant au bash ?
perror("error opening data.csv");
return 1;
}
printf("before d1_data allocate\n");
d1_cdata *data = malloc(lsize*sizeof(d1_cdata)); // contient routeID stepID et name
printf("end allocation d1_data\n");
if (data == NULL) {
perror("error allocating for d1_cdata");
return 1;
}
printf("after data NULL check before extract utils declaration\n");
char line[256]; // a ajuster ou malloc ?
int rcounter = 0;
int rcounterR = 0;
printf("allocated rc /rcR/ line bef compar1/2\n");
int compar1[lsize];
int compar2[lsize];
printf("allocared compar1/2 bef check printf bef while fgets\n");
int x1, x2;
printf("before entry while fgets (reading content\n");
while (fgets(line, sizeof(line), file) != NULL && rcounter < lsize) {

char *token = strtok(line, ";");

//data[rcounter].routeID = atoi(token);
x1 = atoi(token);
printf("assigned x1 to token f1\n");
token = strtok(NULL, ";");
//data[rcounter].stepID = atoi(token);
x2 = atoi(token);
printf("assigned x2 to token f2\n");
compar1[rcounter] = x1;
compar2[rcounter] = x2;
printf("assigned x1 x2 to compar1/2\n");
if (compare(x1, x2, compar1, compar2, rcounter)) {
printf("into if compare validated \n");
data[rcounter].routeID = x1; //ICI
data[rcounter].stepID = x2; // et ici segfault !!
printf("assignated x1 x2 to routeID/step\n");
token = strtok(NULL, ";");
token = strtok(NULL, ";");
token = strtok(NULL, ";");
token = strtok(NULL, ";");
data[rcounter].name[0] = '\0';
strcpy(data[rcounter].name, token);
rcounterR++;
}

else {
printf("doublon detecté et pas ajouté");
}
rcounter++;
fclose(file);
}
printf("BEFORE TRI START\n");
printf("OLD (OBSOLETE) LSIZE : %d\n", lsize);
/*for (int i = 0; i < lsize; i++) {
        for (int j = i+1; j < lsize; j++) {
        if (data[i].routeID == data[j].routeID && data[i].stepID != data[j].stepID) {

                for (int k = j; k < lsize - 1; k++) {
                data[k] = data[k+1];    
                }
                lsize--;
        j--;
        }

        }*/


printf("NEW LSIZE : %d\n", lsize);
/*    FILE *result_file = fopen("result.txt", "w");

    if (result_file == NULL) {
        perror("Error opening result.txt");
        return 1;
    }

    for (int i = 0; i < rcounter; i++) {
       fprintf(result_file, "ROUTEID: %d STEPID %d NAME %s || ROW %d\n", data[i].routeID, data[i].stepID, data[>
    }

    fclose(result_file);

    printf("Data written to result.txt successfully.\n"); */
printf("fin du programe pas de write\n");
free(data);
}
