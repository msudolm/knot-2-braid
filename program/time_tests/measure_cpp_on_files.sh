#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename_list>"
    exit 1
fi

filename_list="$1"

while IFS= read -r filename; do
    ./measure_braiding_only.out "$filename"
done < "$filename_list"