#!/bin/bash

# Vérifier si le nom du fichier CSV est fourni en argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_filename>"
    exit 1
fi

# Chemin complet du fichier CSV
exec_timestart_l=$(date +%s.%N)
csv_file="${1}"

# Calculer la distance totale de chaque trajet et trier les résultats
awk -b -F';' '
    NR > 1 { distance[$1] += $5 }
    END {
        for (route in distance) {
            print distance[route], route
        }
    }
' "$csv_file" | sort -nrk1,1 | head -n 10 | sort -nk2,2 > "temp/top_trajets.dat"

# Afficher les 10 trajets les plus longs triés par identifiant de trajet
echo "Les 10 trajets les plus longs :"
cat "temp/top_trajets.dat"

exec_timeend_l=$(date +%s.%N)
exec_timetotal_l=$(echo "$exec_timeend_l - $exec_timestart_l" | bc)

echo "Temps d'exécution total du script : $exec_timetotal_l secondes"

gnuplot -persist <<-PLOT

set terminal png
set output 'Image/Trajet_Distance.png'
set ylabel 'DISTANCE (km)'
set xlabel 'ROUTE ID'
set title 'Top 10 trajets les plus long'
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 1
set grid ytics
set xtics rotate by -45
set yrange [0:*]
set datafile separator " "
plot 'temp/top_trajets.dat' using 1:xtic(2) notitle lc rgb "green"

PLOT
