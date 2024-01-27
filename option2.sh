#!/bin/bash

# Définir le répertoire de base
home="/home"

# Vérifier si le fichier CSV est fourni en argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_file>"
    exit 1
fi

csv_file="$1"

# Extraction des conducteurs et de la distance totale parcourue
awk -b -F ';' '{ sum[$6] += $5 } END { for (i in sum) print sum[i], i }' "$csv_file" | \
    sort -nr | head -n 10 > "$home/younes/Info/conducteur_distance.dat"

gnuplot -persist <<PLOT

set terminal png size 700,1000
set output 'option2graph.png'
set ylabel 'OPTION -D2'
set y2label 'DISTANCE (KM)' offset -3,0
set xlabel 'DISTANCE (KM)' rotate by  180 offset character 0, -11, 0
set ytics rotate by 90 
set xtics font "Arial, 11" 
set xtic rotate by 90 scale 0 offset character 0, -11, 0
set style data histograms
set style fill solid 0.5 border -1
set boxwidth 1
set grid ytics
unset yrange
set size 0.6, 1
set datafile separator " "
set lmargin 10
set bmargin 15
set rmargin 0.5
set tmargin 5
plot "$home/younes/Info/conducteur_distance.dat" using 1:xtic(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) notitle lc rgb "purple"

PLOT


convert -rotate 90 option2graph.png option2graph.png
exit 0
