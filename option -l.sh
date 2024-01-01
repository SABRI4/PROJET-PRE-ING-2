#!/bin/bash

# Vérifier si le fichier CSV est fourni en argument


if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_file>"
    exit 1
fi
exec_timestart_l=$(date +%s.%N)
csv_file="$1"

# Extraction des ID et de la distance totale parcourue
awk -F ';' '{ sum[$1] += $5 } END { for (i in sum) print sum[i], i }' "$csv_file" | \
    sort -nr | head -n 10 > "ID_Trajet_distance.dat" // pb avc tri : tri doit etre croissant sur colonne n°2 (Route ID) et sort -n -k2 tri incorrectement

# Afficher le contenu du fichier temporaire
echo "Contenu du fichier ID_Trajet_distance.dat :"
echo "Distance  |  ID_Trajet "
cat "ID_Trajet_distance.dat"

exec_timeend_l=$(date +%s.%N)
exec_timetotal_l=$(echo "$exec_timeend_l - $exec_timestart_l" | bc)

echo "Temps d'exécution total du script : $exec_timetotal_l secondes"

gnuplot -persist <<-PLOT

set terminal png
set output 'Trajet_Distance.png'
set ylabel 'DISTANCE (km)'
set xlabel 'ROUTE ID'
set title 'Top 10 trajets les plus long'
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 1
set grid ytics
unset xrange
set datafile separator " "
plot 'ID_Trajet_distance.dat' using 1:xtic(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) notitle lc rgb "green" // pb dans disposition et manipulatiuon histo

PLOT

exit 0
