#!/bin/bash
# Esempio 7: Uso di array

numeri=(10 20 30 40)
echo "Tutti i numeri: ${numeri[@]}"
echo "Il primo numero è: ${numeri[0]}"
echo "Numero di elementi: ${#numeri[@]}"

for n in "${numeri[@]}"; do
  echo "Valore: $n"
done
