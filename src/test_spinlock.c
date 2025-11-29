#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "spinlock.h"  

#define TOTAL_CRIT_SECTIONS 32768
#define DELAY 1000       // pour boucle

spinlock_t spin;

void *worker(void *arg) {
    int iterations = *(int*)arg;

    for (int i = 0; i < iterations; i++) {
        lock(&spin);  

        // section critique
        for (volatile int j = 0; j < DELAY; j++);

        unlock(&spin); 
    }

    return NULL;
}

double get_time_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (ts.tv_nsec / 1e9);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nombre_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "N doit être > 0\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[N];
    int iter = TOTAL_CRIT_SECTIONS / N;

    // Initialisation du spinlock
    spin.locked = 0;

    double start = get_time_sec();

    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, worker, &iter);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = get_time_sec();

    printf("%d, %.6f\n", N, end - start);

    return 0;
}
