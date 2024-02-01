# Projet de Gestion de Données pour une Société de Transport Routier : CYTRUCK

## Description Générale

 êtes le gérant d'une société nationale de transport routier, et vous cherchez à optimiser l'analyse des données de trajets routiers? Ce projet consiste à développer un programme en shell qui analyse un fichier de données volumineux, utilise un programme C pour des traitements spécifiques si nécessaire, puis génère des graphiques résumant les résultats.

## Organisation du Projet

- **data/ :** Dossier contenant le fichier CSV d'entrée avec les données des trajets.??
- **progc/ :** Dossier contenant le programme C, son makefile, et l'exécutable.
- **images/ :** Dossier où seront stockées les images des graphiques générés. ??
- **temp/ :** Dossier pour les fichiers intermédiaires nécessaires au programme. ??
- **demo/ :** Dossier où seront des exemples d’exécution de notre application.
- **cytruck.sh :** Script Shell principal situé à la racine du projet.

## Utilisation du Script Shell

Avant l'utilisation de notre application veuillez vous accorder les droits d'execution de l'application dans votre terminal via la commande suivante :
- chmod +x cytruck.sh

Le script shell prend en paramètres le chemin du fichier CSV d'entrée et les choix de traitements à effectuer. Voici les options disponibles :

- **-h :** Affiche l'aide expliquant les options disponibles.
- **-d1 :** Traite les conducteurs avec le plus de trajets.
- **-d2 :** Traite les conducteurs avec la plus grande distance parcourue.
- **-l :** Traite les 10 trajets les plus longs.
- **-t :** Traite les 10 villes les plus traversées.
- **-s :** Traite les statistiques sur les étapes.

Exemple d'utilisation :

./cytruck.sh chemin/vers/data.csv -l
