#!/bin/bash

# Transferts the input filename to$inputfile
input_file="$1"

# Creates a temporary file for holding the output
temp_file=$(mktemp)

# Check if the file ends with newline character
if [ "$(tail -c1 "$input_file"; echo x)" != $'\nx' ]; then
    echo "Map is clean"
else
    # Copy all lines except the last newline to the temporary file
    head -c -1 "$input_file" > "$temp_file"

    # Replace original file with temporary file
    mv "$temp_file" "$input_file"
    echo "Last newline removed from the file."
fi
