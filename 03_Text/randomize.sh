#!/bin/bash
delay=${1:-0.1}

tput clear

cat > /tmp/ascii_art.tmp

row=0
while IFS= read -r line; do
    col=0
    while [ $col -lt ${#line} ]; do
        char="${line:$col:1}"
        echo "$row $col $char" >> /tmp/coords.tmp
        col=$((col + 1))
    done
    row=$((row + 1))
done < /tmp/ascii_art.tmp

shuf /tmp/coords.tmp > /tmp/shuffled_coords.tmp

while read -r row col char; do
    tput cup $row $col
    echo -n "$char"
    sleep $delay
done < /tmp/shuffled_coords.tmp

total_rows=$(wc -l < /tmp/ascii_art.tmp)
tput cup $total_rows 0
echo

rm /tmp/ascii_art.tmp /tmp/coords.tmp /tmp/shuffled_coords.tmp
