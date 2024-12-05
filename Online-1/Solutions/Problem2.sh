#!/bin/bash

# Function to check if two files are identical, ignoring trailing whitespaces and blank lines
is_identical() {
    local file1="$1"
    local file2="$2"

    diff -wB "$file1" "$file2" >/dev/null 2>&1
}

# Get a list of all files in the Submissions directory
files=(Submissions/*)

# Compare each file with the ones that come after it
for ((i = 0; i < ${#files[@]} - 1; i++)); do
    for ((j = i + 1; j < ${#files[@]}; j++)); do
        if is_identical "${files[i]}" "${files[j]}"; then
            echo "${files[i]##*/} and ${files[j]##*/} are the same"
        fi
    done
done
