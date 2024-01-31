#!/bin/bash

# Vérification de la présence de deux arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <C PROGRAM> <csv_file>"
    exit 1
fi

executable=$1
csv_file=$2

gcc $1 -o code2

# Exécution de l'exécutable pour traiter le fichier CSV et création de temp.dat
./code2 6000000 $csv_file > temp.dat

# Utilisation de awk pour transformer les données en un format attendu par Gnuplot
awk -F '|' '{print $3, $7, $11, $13 }' temp.dat > option-s.dat
# Suppression du fichier temporaire
rm temp.dat

# Vérification de l'existence de données dans option-s.dat
if [ ! -s option-s.dat ]; then
    echo "Erreur : Le fichier de données est vide ou n'existe pas."
    exit 2
fi
cat option-s.dat

# Script Gnuplot
gnuplot -persist <<-EOF
    set terminal png size 1400, 600
    set output 'option-s_graph.png'
    set ylabel 'Distance (km)'
    set xlabel 'ROUTE ID'
    set title 'Option -s: Distance f(Route)'
    set xtics rotate by -90
    set key outside
    set style data lines
    set yrange [0:*]
    unset xrange
    set datafile separator ";"
    plot 'option-s.dat' using 2:xtic(1) title 'Distance average (Km)' with lines, \
         'option-s.dat' using 3:xtic(1) title 'Distances Max (Km)' with lines, \
         'option-s.dat' using 4:xtic(1) title 'Distances Min (Km)' with lines
EOF

exit 0


