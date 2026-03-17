#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

typedef struct {
    int *data;
    size_t n;
} int_vec_t;

static int pow_int(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
}

int_vec_t v = { .data = (int[]){1, 2, 3, 4, 5}, .n = 5 };

/* Turno atomico: atomic_load/store "semplici" => memory_order_seq_cst */
static atomic_int turn = 0;

int norma_i(void *arg) {
    int idx = *(int *)arg;

    // parte indipendente
    int risultato = pow_int(v.data[idx], 2);

    // attesa attiva finché non è il mio turno
    while (atomic_load(&turn) != idx) {
        thrd_yield(); // opzionale: riduce il consumo CPU
    }

    // se sono qui, tocca a me (ordine garantito dal protocollo su turn)
    if (idx > 0)
        risultato += v.data[idx - 1];

    v.data[idx] = risultato;

    // passa il turno al successivo
    atomic_store(&turn, idx + 1);

    return EXIT_SUCCESS;
}

int main(void) {
    thrd_t thread_ids[5];
    int dato_assegnato[5] = {0, 1, 2, 3, 4};

    for (size_t i = 0; i < v.n; i++) {
        if (thrd_create(&thread_ids[i], norma_i, &dato_assegnato[i]) != thrd_success) {
            fprintf(stderr, "Errore: thrd_create (i=%zu)\n", i);
            for (size_t j = 0; j < i; j++) thrd_join(thread_ids[j], NULL);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < v.n; i++)
        thrd_join(thread_ids[i], NULL);

    for (size_t i = 0; i < v.n; i++)
        printf("Il valore calcolato per la posizione %zu è: %d\n", i+1, v.data[i]);

    return EXIT_SUCCESS;
}