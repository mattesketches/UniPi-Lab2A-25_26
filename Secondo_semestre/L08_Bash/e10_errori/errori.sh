#!/bin/bash
# Esempio 6: Gestione errori con &&, || e controllo exit status

ls file_che_non_esiste.txt && echo "Trovato!" || echo "Errore: file non trovato."

cp file_che_non_esiste.txt copia.txt
if [ $? -ne 0 ]; then
  echo "Copia fallita."
fi
