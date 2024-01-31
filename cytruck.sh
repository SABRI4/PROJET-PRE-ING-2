#!/bin/bash

#accorde acces exec à tout les script
chmod +x cytruck.sh
chmod +x optionLV4.sh
chmod +x option1.sh
chmod +x option2.sh
chmod +x options.sh
chmod +x optiont.sh
# Fonction pour afficher l'aide
print_help() {
    echo "Usage: $0 -d1 | -d2 | -t | -s | -l FILE"
    echo "Options:"
    echo "  -d1      Appelle le script optiond1.sh"
    echo "  -d2      Appelle le script optiond2.sh"
    echo "  -t       Appelle le script optiont.sh"
    echo "  -s       Appelle le script options.sh"
    echo "  -l       Appelle le script optionLV4.sh"
}

# Vérification de la présence du dossier temp
check_temp_folder() {
    if [ ! -d "temp" ]; then
        echo "Création du dossier temp"
        mkdir temp
    else
        echo "Le dossier temp existe déjà."
    fi
}

# Vérification de la présence du dossier images
check_images_folder() {
    if [ ! -d "images" ]; then
        echo "Création du dossier images"
        mkdir images
    else
          echo "Le dossier images existe déjà."
    fi
}

# Vérification du nombre d'arguments
if [ "$#" -lt 2 ]; then
    echo "Erreur: Le fichier CSV en argument est manquant."
    print_help
    exit 1
fi

# Vérification de l'existence du fichier CSV
csv_file="$2"

if [ ! -f "$csv_file" ]; then
    echo "Erreur: Le fichier CSV '$csv_file' n'existe pas."
    print_help
    exit 1
fi

# Vérification des options
case "$1" in
    -d1)
        ./option1.sh $2
        ;;
    -d2)
        ./option2.sh $2
        ;;
    -t)
        ./optiont.sh optt.c.sh $2
        ;;
    -s)
        ./options.sh opts.c $2
        ;;
    -l)
        ./optionLV4.sh $2
        ;;
    *)
        echo "Erreur: Option invalide."
        print_help
        exit 1
        ;;
esac

# Vérification des dossiers temp et images
check_temp_folder
check_images_folder

echo "Le script a été exécuté avec succès."
