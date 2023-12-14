bash script pour D1_cfile.c

#!/bin/bash

if [ -n "$1" ] ; then  #ajouter des else pour gerer les erreurs
echo "paramètre 1 éxiste"
fichier_csv="$1"
else 
echo "chemin fichier csv non fournis"
exit
fi

if [ -n "$2" ] ; then
echo "paramètre 2 éxiste"
else
echo "pas de paramètre fournis"
exit
fi

if [ -e "$fichier_csv" ] ; then
echo "fichier csv existe"
csv_size=$(wc -l < "$fichier_csv")
echo "csv file size:"
echo "$csv_size"
echo "data location :"
echo "$fichier_csv"
else
echo "pb fichier csv"
fi

if [ "$2" = '-D1' ] ; then
exec_timestart_D1=$(date +%s.%N)
echo "paramètre 2 = D1"
cd progc || exit
if [ -e "progc/execD1" ] ; then
./execD1 "$fichier_csv" "$csv_size"
ret1=$?
else 
echo "compilation de D1.c"
gcc -o execD1 D1_cfile2.c -Ibib
./execD1 "$fichier_csv" "$csv_size"
ret2=$?
fi
echo "output of the c file : (jsp si utile ou non pour la suite)"
echo "$ret1"
echo "$ret2" 
exec_timeend_D1=$(date +%s.%N) 
exec_timetotal_D1=$(echo "$exec_timestart_D1 - $exec_timeend_D1" | bc)
echo "temps d'execution : $exec_timetotal_D1"

elif [ "$2" = '-h' ] ; then
echo "message d'aide"
fi #mauvais placement des fi?
