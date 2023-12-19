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
    sort -nr | head -n 10 > "$home/younes/Info/conducteur_distance.txt"

# Afficher le contenu du fichier temporaire
echo "Contenu du fichier conducteur_distance.txt :"
cat "$home/younes/Info/conducteur_distance.txt"


# Nettoyer les fichiers temporaires
rm "$home/younes/Info/conducteur_distance.txt"

exit 0
