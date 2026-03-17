#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

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

mtx_t mutex;
cnd_t cond_var;

int_vec_t v = { .data = (int[]){1, 2, 3, 4, 5}, .n = 5 };
int turn = 0; // indice del thread che può procedere

int norma_i(void *arg) {
    int idx = *(int *)arg;

    // parte indipendente (nessuna sincronizzazione necessaria)
    int risultato = pow_int(v.data[idx], 2);

    mtx_lock(&mutex);

    // impone ordine: può procedere solo il thread col turno corrente
    while (turn != idx)
        cnd_wait(&cond_var, &mutex);

    // completa il calcolo in ordine
    if (idx > 0)
        risultato += v.data[idx - 1];

    v.data[idx] = risultato;

    // passa il turno al successivo
    turn++;

    // sveglia chi sta aspettando (broadcast è semplice e sicuro qui)
    cnd_broadcast(&cond_var);

    mtx_unlock(&mutex);
    return EXIT_SUCCESS;
}

int main(void) {
    thrd_t thread_ids[5];
    int dato_assegnato[5] = {0, 1, 2, 3, 4}; // indici: ogni thread riceve il proprio

    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        fprintf(stderr, "Errore: mtx_init\n");
        return EXIT_FAILURE;
    }
    if (cnd_init(&cond_var) != thrd_success) {
        fprintf(stderr, "Errore: cnd_init\n");
        mtx_destroy(&mutex);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < v.n; i++) {
        if (thrd_create(&thread_ids[i], norma_i, &dato_assegnato[i]) != thrd_success) {
            fprintf(stderr, "Errore: thrd_create (i=%zu)\n", i);
            // join dei thread già creati
            for (size_t j = 0; j < i; j++) thrd_join(thread_ids[j], NULL);
            cnd_destroy(&cond_var);
            mtx_destroy(&mutex);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < v.n; i++)
        thrd_join(thread_ids[i], NULL);

    for (size_t i = 0; i < v.n; i++)
        printf("Il valore calcolato per la posizione %zu è: %d\n", i+1, v.data[i]);

    cnd_destroy(&cond_var);
    mtx_destroy(&mutex);
    return EXIT_SUCCESS;
}