#!/bin/bash

#répertoire home
home="/home"

if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_file>"
    exit 1
fi

csv_file="$1"

cut -d ';' -f 1,6 "$csv_file" | sort | uniq -c | sort -nr > "$home/younes/Info/conducteurstrajets.txt"

top_conducteurs=$(head -n 10 "$home/younes/Info/conducteurstrajets.txt")

echo "Top 10 des conducteurs avec le plus de trajets différents:"
echo "$top_conducteurs" | while read count name route_id; do
    printf "%-6s %-20s %-10s\n" "$count" "$name" "$route_id"
done

rm "$home/younes/Info/conducteurstrajets.txt"

exit 0
