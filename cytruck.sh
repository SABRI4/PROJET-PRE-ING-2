#!/bin/bash

#accorde acces exec à tout les script
chmod +x cytruck.sh
chmod +x option-l.sh
chmod +x option1.sh
chmod +x option2.sh
chmod +x option-s.sh
chmod +x option-t.sh

# Fonction pour afficher l'aide
print_help() {
    echo "Usage: $0 -d1 | -d2 | -t | -s | -l <FILE>"
    echo "Options:"
    echo "  -d1      Appelle le script optiond1.sh"
    echo "  -d2      Appelle le script optiond2.sh"
    echo "  -t       Appelle le script optiont.sh"
    echo "  -s       Appelle le script options.sh"
    echo "  -l       Appelle le script optionLV4.sh"
}


# Vérification de la présence du dossier temp
check_temp_folder() {
    if [ ! -e "temp" ]; then
        echo "Création du dossier temp"
        mkdir -p temp
    else
        
        echo "Le dossier temp existe déjà."
    fi
}

# Vérification de la présence du dossier data
check_data_folder() {
    if [ ! -e "data" ]; then
        echo "Création du dossier data"
        mkdir -p data
    else
        
        echo "Le dossier data existe déjà."
    fi
}

# Vérification de la présence du dossier images
check_images_folder() {
    if [ ! -e "images" ]; then
        echo "Création du dossier images"
        mkdir -p images
    else
        
        echo "Le dossier images existe déjà."
    fi
}

# Vérification du nombre d'arguments
if [ "$#" -ne 2 ]; then
    echo "Erreur: il y a trop d'arguments dans l'argumentation, veuillez utiliser -h pour plus d'aide"
    print_help
    exit 1
fi

# Vérification de l'existence du fichier CSV
csv_file="$1"

if [ ! -f "$csv_file" ]; then
    echo "Erreur: Le fichier CSV '$csv_file' n'existe pas."
    print_help
    exit 1
fi

#verification de la présence des dossier data, temp et images
check_temp_folder
check_data_folder
check_images_folder

# Vérification des options
case "$2" in
    -d1)
        ./option1.sh $1
        ;;
    -d2)
        ./option2.sh $1
        ;;
    -t)
        ./option-t.sh optt $1
        make clean

        ;;
    -s)
        ./option-s.sh opts $1
        make clean

        ;;
    -l)
        ./option-l.sh $1
        ;;
    *)
        echo "Erreur: Option invalide."
        print_help
        exit 1
        ;;
esac

echo "Le script a été exécuté avec succès."
