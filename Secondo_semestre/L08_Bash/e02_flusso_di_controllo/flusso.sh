#!/bin/bash

# Esempio di flusso di controllo in Bash

echo "Esempio di if-else:"
read -p "Inserisci un numero: " num
if [ "$num" -gt 0 ]; then
	echo "Il numero è positivo."
elif [ "$num" -lt 0 ]; then
	echo "Il numero è negativo."
else
	echo "Il numero è zero."
fi

echo
echo "Esempio di ciclo for:"
for i in 1 2 3 4 5; do
	echo "Valore di i: $i"
done

echo
echo "Esempio di ciclo while:"
count=1
while [ $count -le 3 ]; do
	echo "Contatore: $count"
	((count++))
done

echo
echo "Esempio di case:"
read -p "Inserisci una lettera (a/b/c): " lettera
case "$lettera" in
	a)
		echo "Hai scelto a."
		;;
	b)
		echo "Hai scelto b."
		;;
	c)
		echo "Hai scelto c."
		;;
	*)
		echo "Scelta non valida."
		;;
esac