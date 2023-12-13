#!/bin/bash

fichier_csv="data/data.csv"

if [ -n "$1" ] ; then
echo "paramètre 1 éxiste"



if [ "$1" = '-D1' ] ; then
exec_timestart_D1=$(date +%s.%N)
echo "paramètre 1 = D1"
cut -d';' -f1,6 "$fichier_csv" | uniq | uniq -f1 -c | awk '{print $1 ";" $2 " " $3}' | cut -d';' -f1,3 | sort -rn | head -n10 > "temp/D1_temp_list.txt"
exec_timeend_D1=$(date +%s.%N) 
exec_timetotal_D1=$(echo "$exec_timestart_D1 - $exec_timeend_D1" | bc)
echo "temps d'execution : $exec_timetotal_D1"

elif [ "$1" = '-h' ] ; then
echo "message d'aide"
fi
fi



#!/bin/bash

fichier_csv="data/data.csv"

if [ -n "$1" ] ; then
    echo "paramètre 1 existe"

    if [ "$1" = '-D1' ] ; then
        exec_timestart_D1=$(date +%s.%N)
        echo "paramètre 1 = D1"
        
        # Manipulation des données en une seule commande
        awk -F';' '{count[$1" "$6]++} END {for (word in count) print count[word], word}' "$fichier_csv" | sort -rn | head -n10 > "temp/D1_temp_list.txt"
        
        exec_timeend_D1=$(date +%s.%N) 
        exec_timetotal_D1=$(echo "$exec_timeend_D1 - $exec_timestart_D1" | bc)
        echo "temps d'execution : $exec_timetotal_D1"

    elif [ "$1" = '-h' ] ; then
        echo "message d'aide"
    fi
fi

// fichier 3

#!/bin/bash

fichier_csv="data/data.csv"

if [ -n "$1" ] ; then
    echo "paramètre 1 existe"

    if [ "$1" = '-D1' ] ; then
        exec_timestart_D1=$(date +%s.%N)
        echo "paramètre 1 = D1"

        # Manipulation des données en une seule commande
        awk -F';' '!seen[$6]++ {print $6}' "$fichier_csv" | awk '{count[$1]++} END {for (word in count) print count[word], word}' | sort -rn | head -n10 > "temp/D1_temp_list.txt"
        
        exec_timeend_D1=$(date +%s.%N) 
        exec_timetotal_D1=$(echo "$exec_timeend_D1 - $exec_timestart_D1" | bc)
        echo "temps d'execution : $exec_timetotal_D1"

    elif [ "$1" = '-h' ] ; then
        echo "message d'aide"
    fi
fi


