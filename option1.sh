#!/bin/bash

#départ chrono
exec_timestart_1=$(date +%s.%N)

# Vérifier si le nom du fichier CSV est fourni en argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_filename>"
    exit 1
fi

# Chemin complet du fichier CSV
csv_file=$1

# Chemin complet du fichier de sortie
output_file="data/conducteur_trajets.dat"

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

cat $output_file

#Fin chrono
exec_timeend_1=$(date +%s.%N)
exec_timetotal_1=$(echo "$exec_timeend_1 - $exec_timestart_1" | bc)

echo "Temps d'exécution total du script : $exec_timetotal_1 secondes"

# Création de l'histogramme avec Gnuplot
gnuplot -persist <<-PLOT

set terminal png size 700,1000
set output 'images/conducteurs_histogramme.png'
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
plot "$output_file" using 3:xticlabels(sprintf("%s %s", stringcolumn(1), stringcolumn(2))) >

PLOT

convert -rotate 90 images/conducteurs_histogramme.png  images/conducteurs_histogramme.png

exit 0
