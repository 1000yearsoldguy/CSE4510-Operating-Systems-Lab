#!/bin/bash

# Default values
DEFAULT_MAX_SCORE=100
DEFAULT_MAX_STUDENT_ID=5

# Prompting user for max_score and validating the input
while ! [[ "$max_score" =~ ^[0-9]+$ ]] || [[ "$max_score" -le 0 ]]; do
  if [[ -z "$1" ]]; then
    read -p "Enter max_score (default is $DEFAULT_MAX_SCORE): " max_score
    max_score=${max_score:-$DEFAULT_MAX_SCORE}
  else
    max_score="$1"
  fi

  if ! [[ "$max_score" =~ ^[0-9]+$ ]] || [[ "$max_score" -le 0 ]]; then
    echo "Error: max_score must be a positive integer."
  fi
done

# Prompting user for max_student_id and validating the input
while [[ ! "$max_student_id" =~ ^[1-9]$ ]]; do
  if [[ -z "$2" ]]; then
    read -p "Enter max_student_id (1-9, default is $DEFAULT_MAX_STUDENT_ID): " max_student_id
    max_student_id=${max_student_id:-$DEFAULT_MAX_STUDENT_ID}
  else
    max_student_id="$2"
  fi

  if ! [[ "$max_student_id" =~ ^[1-9]$ ]]; then
    echo "Error: max_student_id must be a positive integer between 1 and 9 (inclusive)."
  fi
done

# Creating or rewriting output.csv file with 2 columns - Student ID and Score
echo "Student ID","Score" > output.csv

# Student ID Initialization and path decide
for ((i=1; i<=max_student_id; i++)); do
  student_id="180512$i"
  student_dir="Submissions/$student_id"
  script_file="$student_dir/$student_id.sh"
  output_file="$student_dir/output.txt"

  # Check if the student's directory and script file exist
  if [[ ! -d "$student_dir" ]] || [[ ! -f "$script_file" ]]; then
      echo "$student_id,0" >> output.csv
      continue
  fi

  # Execute the student's script and capture the output
  bash "$script_file" > "$output_file" 2>/dev/null

  # Compare the output with the expected output
  mismatches=$(diff -iwB "$output_file" AcceptedOutput.txt | grep -E '<|>' | wc -l)

  # Calculating the score
  deduction=$((mismatches * 5))
  score=$((max_score - deduction))
  if [[ $score -lt 0 ]]; then
    score=0
  fi

  # Appending results to CSV file
  echo "$student_id","$score" >> output.csv
done

echo "The Evaluation Results are saved in output.csv File"