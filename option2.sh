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



# Afficher le contenu du fichier temporaire
echo "Contenu du fichier conducteur_distance.dat :"
cat "$home/younes/Info/conducteur_distance.dat"

gnuplot -persist <<PLOT

set terminal png
set output 'option2graph.png'
set ylabel 'DISTANCE (Km)'  # This now becomes the categorical axis
set xlabel 'DRIVERS NAMES'  # This now becomes the numerical axis
set title 'Option -d2 : Distance = f(Driver)'
set style data histograms
set style histogram rowstacked  # This will stack the bars in one row
set style fill solid border -1
set boxwidth 0.65 relative
set grid ytics
set xtics rotate by -45
unset yrange  # This allows Gnuplot to auto-scale the y-axis
set datafile separator " "
plot "$home/younes/Info/conducteur_distance.dat" using 1:xtic(sprintf("%s %s", stringcolumn(2), stringcolumn(3))) notitle lc rgb "purple"

PLOT

exit 0
