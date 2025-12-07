#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "my_semaphore.h"

my_semaphore_t *sem;
int N_PHILOSOPHES;

void* philosophe_action(void* arg) {
    long id = (long)arg;
    int left = id;
    int right = (id + 1) % N_PHILOSOPHES;

    for (long i = 0; i < 1000000; i++) {
        // penser
        
        // prendre fourchettes
        if (id == N_PHILOSOPHES - 1) {
            my_sem_wait(&sem[right]);
            my_sem_wait(&sem[left]);
        } else {
            my_sem_wait(&sem[left]);
            my_sem_wait(&sem[right]);
        }

        // manger

        // poser fourchettes
        my_sem_post(&sem[left]);
        my_sem_post(&sem[right]);
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
    sem = malloc(N_PHILOSOPHES * sizeof(my_semaphore_t));

    // init
    for (int i = 0; i < N_PHILOSOPHES; i++) {
        my_sem_init(&sem[i], 1);
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
    free(threads);
    free(sem);
    double time = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1000000000.0;
    printf("%.6f\n", time);

    return 0;
}
