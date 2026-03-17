#!/bin/bash

# Esempio di uso di espressioni regolari nella guardia di un if in bash

read -p "Inserisci una stringa: " input

# Verifica se la stringa contiene solo lettere maiuscole
if [[ "$input" =~ ^[A-Z]+$ ]]; then
	echo "La stringa contiene solo lettere maiuscole."
else
	echo "La stringa NON contiene solo lettere maiuscole."
fi

# Verifica se la stringa contiene solo lettere maiuscole
if [[ "$input" =~ ^[[:upper:]]+$ ]]; then
	echo "La stringa contiene solo lettere maiuscole."
else
	echo "La stringa NON contiene solo lettere maiuscole."
fi

# Verifica se la stringa è un numero intero
if [[ "$input" =~ ^-?[0-9]+$ ]]; then
	echo "La stringa è un numero intero."
else
	echo "La stringa NON è un numero intero."
fi	


# Verifica se la stringa è un numero intero
if [[ "$input" =~ ^[[:digit:]]+$ ]]; then
	echo "La stringa è un numero intero."
else
	echo "La stringa NON è un numero intero."
fi	