#!/bin/bash

home="/home"

if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_file>"
    exit 1
fi

csv_file="$1"

# Process CSV file and get the top 10 drivers
top_conducteurs=$(cut -d ';' -f 1,6 "$csv_file" | sort | uniq -c | sort -nr | head -n 10)

echo "Top 10 des conducteurs avec le plus de trajets différents:"
echo "$top_conducteurs" | while read count name route_id; do
    printf "%-6s %-20s %-10s\n" "$count" "$name" "$route_id"
done

# Save data to a file for Gnuplot
echo "$top_conducteurs" | awk '{print $3, $1}' > "$home/younes/Info/conducteurstrajets.dat"

# Generate Gnuplot script
gnuplot_script="$home/younes/Info/plot_script.gnu"
cat > "$gnuplot_script" << EOL
set terminal png
set output 'conducteurstrajets_plot.png'
set title 'Top 10 Conducteurs avec le Plus de Trajets Différents'
set xlabel 'Nombre de Trajets'
set ylabel 'Conducteurs'
set style fill solid
set boxwidth 0.42
set ytics nomirror
set yrange [0:60]
set xtics rotate by -45
plot "$home/younes/Info/conducteurstrajets.dat" using 2:xtic(1) with boxes title 'Nombre de Trajets'
EOL

# Run Gnuplot
gnuplot "$gnuplot_script"

# Open the PNG file
eog "$home/younes/Info/conducteurstrajets_plot.png" 


# Display completion message
echo "Graphique créé avec succès : conducteurstrajets_plot.png"

# Clean up temporary files
rm "$home/younes/Info/conducteurstrajets.dat" "$gnuplot_script"

exit 0
