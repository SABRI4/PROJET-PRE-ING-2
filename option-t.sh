#!/bin/bash

#chrono depart
exec_timestart_l=$(date +%s.%N)

# Vérification de la présence de deux arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <C PROGRAM> <csv_file>"
    exit 1
fi

executable=$1
csv_file=$2

lines=$(wc -l $2)
# Exécution de l'exécutable pour traiter le fichier CSV et création de temp.dat
if [ ! -e progc/optt ] ;then
cd progc/
make optt
cd -
./optt $lines $csv_file > temp/temp.dat
elif [ -e progc/optt ] ;then
cd progc/
./optt $lines $csv_file > temp/temp.dat
cd -
fi

# Utilisation de awk pour transformer les données en un format attendu par Gnuplot
awk -F '|' '{print $2, $4, $6}' temp/temp.dat > data/option-t.dat
# Suppression du fichier temporaire
rm temp/temp.dat

# Vérification de l'existence de données dans option-s.dat
if [ ! -s option-t.dat ]; then
    echo "Erreur : Le fichier de données est vide ou n'existe pas."
    exit 2
fi


#fin chrono
exec_timeend_l=$(date +%s.%N)
exec_timetotal_l=$(echo "$exec_timeend_l - $exec_timestart_l" | bc)
echo "Temps d'exécution total du script : $exec_timetotal_l secondes"

gnuplot -persist <<-EOF
    set terminal png size 1000,800
    set output 'images/option-t_graph.png'
    set ylabel 'NB ROUTES'
    set xlabel 'TOWN NAMES'
    set title 'OPTION-T'
    set style data histograms
    set style histogram cluster gap 1
    set style fill solid 1.0 border -1
    set boxwidth 1 relative
    set datafile separator ";"
    set xtics rotate by -50 font ",12"  # Adjust font size if necessary
    set lmargin 10  # Adjust left margin
    set rmargin 10  # Adjust right margin
    set tmargin 5   # Adjust top margin
    set bmargin 9   # Adjust bottom margin
    set grid y
    set yrange [0:3500]  # Set y-axis range
    plot 'data/option-t.dat' using 2:xticlabels(1) title 'Town Routes' lc rgb "blue", '' us>EOF

exit 0
