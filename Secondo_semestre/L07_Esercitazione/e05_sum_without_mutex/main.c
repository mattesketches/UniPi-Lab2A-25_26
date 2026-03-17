#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

enum { N = 10000, T = 4 };

typedef struct {
    const int *a;
    size_t n;
    int tid;
    long *partials;   // partials[tid] è l'unico long che il thread può aggiornare
} worker_arg_t;

int worker_sum(void *arg) {
    worker_arg_t *w = (worker_arg_t *)arg;

    // Partizionamento "a blocchi" (contiguo)
    size_t chunk = (w->n + T - 1) / T;         // ceil(n/T)
    size_t start = (size_t)w->tid * chunk;
    size_t end   = start + chunk;
    if (end > w->n) end = w->n;

    long s = 0;  // variabile locale (non conta nel vincolo "un contatore per thread" se intendi contatori condivisi)
    for (size_t i = start; i < end; i++) {
        s += w->a[i];
    }

    // Scrittura su una cella di proprietà esclusiva del thread => no race
    w->partials[w->tid] = s;

    return thrd_success;
}

int main(void) {
    static int a[N];

    // inizializzazione di esempio (puoi sostituire con random o input)
    for (size_t i = 0; i < N; i++) a[i] = 1;   // somma attesa = 10000

    thrd_t tids[T];
    worker_arg_t args[T];

    // "massimo un contatore long per thread"
    // => un array di 4 long, uno per thread.
    long partials[T] = {0};

    for (int t = 0; t < T; t++) {
        args[t] = (worker_arg_t){
            .a = a,
            .n = N,
            .tid = t,
            .partials = partials
        };

        if (thrd_create(&tids[t], worker_sum, &args[t]) != thrd_success) {
            fprintf(stderr, "Errore: thrd_create (t=%d)\n", t);
            // join di quelli già creati
            for (int j = 0; j < t; j++) thrd_join(tids[j], NULL);
            return EXIT_FAILURE;
        }
    }

    for (int t = 0; t < T; t++)
        thrd_join(tids[t], NULL);

    // Riduzione finale sequenziale: nessun mutex necessario
    long total = 0;
    for (int t = 0; t < T; t++)
        total += partials[t];

    printf("Somma totale = %ld\n", total);

    return EXIT_SUCCESS;
}