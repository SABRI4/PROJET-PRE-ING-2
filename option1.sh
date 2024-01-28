#!/bin/bash

# Définir le répertoire de base où se trouve le fichier CSV
home="/home/younes/Info"

# Vérifier si le nom du fichier CSV est fourni en argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_filename>"
    exit 1
fi

# Chemin complet du fichier CSV
csv_file="${home}/${1}"

# Chemin complet du fichier de sortie
output_file="${home}/conducteur_trajets.dat"

awk -F';' '
    NR > 1 {
        if (!(($6 SUBSEP $1) in unique)) {
            unique[$6 SUBSEP $1]
            count[$6]++
        }
    }
    END {
        for (driver in count) {
            print count[driver], driver
        }
    }
' "$csv_file" | sort -nrk1,1 | head -n 10 | awk '{ print $2, $3, $1 }' > "$output_file"

 
# Création de l'histogramme avec Gnuplot
gnuplot -persist <<-PLOT

set terminal png size 700,1000
set output 'conducteurs_histogramme.png'
set ylabel 'TOP 10 CONDUCTEURS AVEC LE PLUS DE TRAJETS'
set y2label 'NB ROUTES' offset 3,0
set xtics font "Arial, 11" 
set xlabel 'DRIVERS NAMES' rotate by  180 offset character 0, -11, 0
set style data histograms
set xtic rotate by 90 scale 0 offset character 0, -11, 0
set style fill solid 0.5 border -1
set boxwidth 0.5
set ytics rotate by 90 
set grid ytics
set yrange [0:*]
set size 0.6, 1
set datafile separator " "
set lmargin 10
set bmargin 15
set rmargin 0
set tmargin 5
plot "/home/younes/Info/conducteur_trajets.dat" using 3:xticlabels(sprintf("%s %s", stringcolumn(1), stringcolumn(2))) notitle lc rgb "blue" with boxes

PLOT

convert -rotate 90 conducteurs_histogramme.png  conducteurs_histogramme.png


exit 0

exit 0
