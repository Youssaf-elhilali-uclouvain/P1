#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t *forks;
int N_PHILOSOPHES;

void* philosophe_action(void* arg) {
    long id = (long)arg;
    int fork_left = id;
    int fork_right = (id + 1) % N_PHILOSOPHES;

    for (long cycle = 0; cycle < 1000000; cycle++) {
        // Penser (immédiat - rien à faire)
        
        // Prendre les fourchettes (anti-deadlock : dernier philosophe inverse l'ordre)
        if (id == N_PHILOSOPHES - 1) {
            pthread_mutex_lock(&forks[fork_right]);
            pthread_mutex_lock(&forks[fork_left]);
        } else {
            pthread_mutex_lock(&forks[fork_left]);
            pthread_mutex_lock(&forks[fork_right]);
        }

        // Manger (immédiat - rien à faire)

        // Reposer les fourchettes
        pthread_mutex_unlock(&forks[fork_left]);
        pthread_mutex_unlock(&forks[fork_right]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    N_PHILOSOPHES = atoi(argv[1]);
    pthread_t *threads = malloc(N_PHILOSOPHES * sizeof(pthread_t));
    forks = malloc(N_PHILOSOPHES * sizeof(pthread_mutex_t));

    for (int i = 0; i < N_PHILOSOPHES; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < N_PHILOSOPHES; i++) {
        pthread_create(&threads[i], NULL, philosophe_action, (void*)i);
    }

    for (int i = 0; i < N_PHILOSOPHES; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    for (int i = 0; i < N_PHILOSOPHES; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    free(threads);
    free(forks);

    double elapsed_sec = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed_sec);

    return 0;
}