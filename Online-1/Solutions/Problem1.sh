#!/bin/bash

# Create the "Rootkits" directory if it doesn't exist
mkdir -p Rootkits

# Iterate through each .txt file in the current directory
for file in *.txt; do
    # Check if the file contains the word "rootkit"
    if grep -q "rootkit" "$file"; then
        # Extract the rootkit name from the last line of the file
        str=$(tail -1 "$file")

        # Move the file to the "Rootkits" directory and rename it
        mv "$file" "Rootkits/${str}.txt"
    fi
done