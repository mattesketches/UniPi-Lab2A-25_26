#!/bin/bash

# Esempio di redirezione STDIN, STDOUT e STDERR

# Redirezione STDOUT su un file
echo "Questo va su output.txt" > output.txt

# Redirezione STDERR su un file
ls file_che_non_esiste 2> error.txt

# Redirezione sia STDOUT che STDERR su un file
ls /etc/cartella_inesistente > all_output.txt 2>&1

# Redirezione STDIN da un file
read prima_linea < output.txt
echo "La prima linea letta da output.txt: $prima_linea"

# Esempio di pipeline con redirezione
cat output.txt | grep "Questo" > trovato.txt

# Aggiunta (append) a un file
echo "Nuova riga" >> output.txt