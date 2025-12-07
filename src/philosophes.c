#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t *forks;
int N_PHILOSOPHES;

void* philosophe_action(void* arg) {
    long id = (long)arg;
    int left = id;
    int right = (id + 1) % N_PHILOSOPHES;

    for (long i = 0; i < 1000000; i++) {
        // penser
        
        // prendre fourchettes
        if (id == N_PHILOSOPHES - 1) {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        } else {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        }

        // manger

        // poser fourchettes
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
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

    struct timespec debut,fin;
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

    //on affiche le temps entre le début et la fin
    double temps = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    printf("%.6f\n", temps);

    return 0;
}