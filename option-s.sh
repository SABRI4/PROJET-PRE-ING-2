#!/bin/bash

#chrono depart
exec_timestart_l=$(date +%s.%N)

# Vérification de la présence de deux arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <C PROGRAM> <csv_file>"
    exit 1
fi

executable=$1
csv_file=$2
lines=$(wc -l $2)

# Exécution de l'exécutable pour traiter le fichier CSV et création de temp.dat
if [ ! -e progc/opts ] ;then
cd progc/
make opts
./opts $lines $csv_file > temp/temp.dat
cd -
elif [ -e progc/opts ] ;then
cd progc/
./opts $lines $csv_file > temp/temp.dat
cd -
fi


# Utilisation de awk pour transformer les données en un format attendu par Gnuplot
awk -b -F '|' '{print $3, $7, $11, $13 }' temp/temp.dat > data/option-s.dat
# Suppression du fichier temporaire
rm temp/temp.dat

# Vérification de l'existence de données dans option-s.dat
if [ ! -s option-s.dat ]; then
    echo "Erreur : Le fichier de données est vide ou n'existe pas."
    exit 2
fi

#fin chrono
exec_timeend_l=$(date +%s.%N)
exec_timetotal_l=$(echo "$exec_timeend_l - $exec_timestart_l" | bc)

echo "Temps d'exécution total du script : $exec_timetotal_l secondes"

# Script Gnuplot
gnuplot -persist <<-EOF
    set terminal png size 1400, 600
    set output 'image/option-s_graph.png'
    set ylabel 'Distance (km)'
    set xlabel 'ROUTE ID'
    set title 'Option -s: Distance f(Route)'
    set xtics rotate by -90
    set key outside
    set style data lines
    set yrange [0:*]
    unset xrange
    set datafile separator ";"
    plot 'data/option-s.dat' using 2:xtic(1) title 'Distance average (Km)' with lines, \
         'data/option-s.dat' using 3:xtic(1) title 'Distances Max (Km)' with lines, \
         'data/option-s.dat' using 4:xtic(1) title 'Distances Min (Km)' with lines
EOF

exit 0