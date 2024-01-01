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

# Utiliser awk pour traiter le fichier CSV
awk -F';' '
    NR > 1 {
        # Créer une clé unique pour chaque combinaison de conducteur et de Route ID
        if (!(($6 SUBSEP $1) in unique)) {
            unique[$6 SUBSEP $1]
            count[$6]++
        }
    }
    END {
        # Imprimer les résultats pour chaque conducteur
        for (driver in count) {
            print count[driver], driver
        }
    }
' "$csv_file" | sort -nrk1,1 | head -n 10 > "$output_file"

# Afficher le contenu du fichier de sortie
echo "Contenu du fichier conducteur_trajets.dat :"
cat "$output_file"


# Création de l'histogramme avec Gnuplot
gnuplot -persist <<-PLOT

set terminal png
set output 'conducteurs_histogramme.png'
set ylabel 'NB ROUTES'
set xlabel 'DRIVERS NAMES'
set title 'Top 10 conducteurs avec le plus de trajets'
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 1
set grid ytics
unset yrange
set xtics rotate by -45 
set datafile separator " "
plot "$home/conducteur_trajets.dat" using 1:xtic(sprintf("%s %s", stringcolumn(2), stringcolumn(3)))  notitle lc rgb "blue"

PLOT

exit 0
