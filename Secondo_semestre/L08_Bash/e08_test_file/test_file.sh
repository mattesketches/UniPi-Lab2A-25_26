#!/bin/bash
# Esempio 4: Test su file

file="test.txt"
if [ -e "$file" ]; then
  echo "Il file $file esiste."
else
  echo "Il file $file NON esiste."
fi

if [ -d "$file" ]; then
  echo "$file è una directory."
else
  echo "$file NON è una directory."
fi
