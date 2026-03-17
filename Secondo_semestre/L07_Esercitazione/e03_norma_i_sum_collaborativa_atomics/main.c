#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <math.h>
#include <stdatomic.h>

typedef struct {
	const int *data;
	int norma;
	size_t n;
} int_vec_t;

static int pow_int(int base, int exp) {
	int result = 1;
	for (int i = 0; i < exp; i++)
		result *= base;
	return result;
}

int_vec_t v = { .data = (const int []){1, 2, 3, 4, 5}, .n = 5, .norma = 2 };

atomic_int sum;

int norma_i(void *arg) {
	int dato = *(int *)arg;

	int risultato = pow_int(v.data[dato], v.norma);
	*(int *)arg = risultato; // Aggiorna il dato con il risultato calcolato
	
	atomic_fetch_add(&sum, risultato);
	
	return EXIT_SUCCESS;
}

int main (int argc, char *argv[]) {
	thrd_t thread_ids[5]; 					 // Array per memorizzare gli ID dei thread
	int dato_assegnato[5] = {0, 1, 2, 3, 4}; // Array per memorizzare le posizioni dei thread

	
	for (size_t i = 0; i < v.n; i++)
		thrd_create(&thread_ids[i], norma_i, &dato_assegnato[i]);	
	
	for (size_t i = 0; i < v.n; i++)
		thrd_join(thread_ids[i], NULL); // Attende che tutti i thread terminino	
	

	double rt_sum = pow(sum, 1.0 / v.norma); // Calcola la radice della somma
	
	printf("La norma %d è: %f\n", v.norma, rt_sum);
	
	return EXIT_SUCCESS;
}