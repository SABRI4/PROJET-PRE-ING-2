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
awk -F ';' '{ sum[$6] += $5 } END { for (i in sum) print sum[i], i }' "$csv_file" | \
    sort -nr | head -n 10 > "$home/younes/Info/conducteur_distance.dat"



# Afficher le contenu du fichier temporaire
echo "Contenu du fichier conducteur_distance.dat :"
cat "$home/younes/Info/conducteur_distance.dat"

gnuplot -persist <<PLOT

set terminal png size 1280,720
set output 'option_d2.png'
set style data histograms
set style fill solid border -1
set boxwidth 2 relative
unset key
set ytics nomirror
set xtics rotate by 90 offset 1,.2 right
set y2tics rotate by 90 offset 0,-1.5
set y2range [0:]
set grid ytics lt 0 lw 1 lc rgb "green"  # Pour les lignes horizontales
set grid xtics lt 0 lw 1 lc rgb "green"  # Pour les lignes verticales
set grid y
set xlabel "DRIVERS NAMES" rotate by 180
set y2label "NB ROUTES"
set ylabel "Option -d1 : Nb routes" rotate by 90 offset 3,0 
plot 'output.txt' using 2:xticlabels(1) notitle axes x1y2 

PLOT





exit 0
