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

# Calculer la distance totale de chaque trajet et trier les résultats
awk -F';' '
    NR > 1 { distance[$1] += $5 }
    END {
        for (route in distance) {
            print distance[route], route
        }
    }
' "$csv_file" | sort -nrk1,1 | head -n 10 | sort -nk2,2 > "${home}/top_trajets.dat"

# Afficher les 10 trajets les plus longs triés par identifiant de trajet
echo "Les 10 trajets les plus longs :"
cat "${home}/top_trajets.dat"
xticlabels
