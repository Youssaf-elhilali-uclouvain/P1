#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "my_semaphore.h"
#include "spinlock.h"

#define BUFFER_SIZE 8

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
spinlock_t mutex;
my_semaphore_t empty;
my_semaphore_t full;
int N_PROD;
int N_CONS;

void* producteur(void* arg) {
    long id = (long)arg;
    int nb = 131072 / N_PROD;
    
    for (int i = 0; i < nb; i++) {
        // traitement
        for (int j = 0; j < 10000; j++);

        my_sem_wait(&empty);
        lock(&mutex);

        buffer[in] = (int)id;
        in = (in + 1) % BUFFER_SIZE;

        unlock(&mutex);
        my_sem_post(&full);
    }
    return NULL;
}

void* consommateur(void* arg) {
    (void)arg;
    int nb = 131072 / N_CONS;
    
    for (int i = 0; i < nb; i++) {
        my_sem_wait(&full);
        lock(&mutex);

        int item = buffer[out];
        (void)item;
        out = (out + 1) % BUFFER_SIZE;

        unlock(&mutex);
        my_sem_post(&empty);

        // traitement
        for (int j = 0; j < 10000; j++);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N_Producteurs> <N_Consommateurs>\n", argv[0]);
        return 1;
    }
    N_PROD = atoi(argv[1]);
    N_CONS = atoi(argv[2]);
    pthread_t *prod = malloc(N_PROD * sizeof(pthread_t));
    pthread_t *cons = malloc(N_CONS * sizeof(pthread_t));

    // init
    mutex.locked = 0;
    my_sem_init(&empty, BUFFER_SIZE);
    my_sem_init(&full, 0);

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < N_PROD; i++) {
        pthread_create(&prod[i], NULL, producteur, (void*)i);
    }

    for (long i = 0; i < N_CONS; i++) {
        pthread_create(&cons[i], NULL, consommateur, (void*)i);
    }

    for (int i = 0; i < N_PROD; i++) {
        pthread_join(prod[i], NULL);
    }

    for (int i = 0; i < N_CONS; i++) {
        pthread_join(cons[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    free(prod);
    free(cons);

    double time = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1000000000.0;
    printf("%.6f\n", time);

    return 0;
}
