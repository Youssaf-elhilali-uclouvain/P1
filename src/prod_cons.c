#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 8

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int items_produced = 0;
int items_consumed = 0;
int Nbre_prod;
int Nbre_cons;

void* producteur(void* arg) {
    long id = (long)arg;
    int nb = 131072 / Nbre_prod;
    
    for (int i = 0; i < nb; i++) {
        // traitement
        for (int j = 0; j < 10000; j++);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = (int)id;
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consommateur() {
    int nb = 131072 / Nbre_cons;
    
    for (int i = 0; i < nb; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // traitement
        for (int j = 0; j < 10000; j++);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Nbre_producteurs> <Nbre_consommateurs>\n", argv[0]);
        return 1;
    }

    Nbre_prod = atoi(argv[1]);
    Nbre_cons = atoi(argv[2]);

    pthread_t *prod = malloc(Nbre_prod * sizeof(pthread_t));
    pthread_t *cons = malloc(Nbre_cons * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    struct timespec debut,fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < Nbre_prod; i++) {
        pthread_create(&prod[i], NULL, producteur, (void*)i);
    }

    for (long i = 0; i < Nbre_cons; i++) {
        pthread_create(&cons[i], NULL, consommateur, (void*)i);
    }

    for (int i = 0; i < Nbre_prod; i++) {
        pthread_join(prod[i], NULL);
    }

    for (int i = 0; i < Nbre_cons; i++) {
        pthread_join(cons[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(prod);
    free(cons);

    //on affiche le temps entre le début et la fin
    double temps = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    printf("%.6f\n", temps);

    return 0;
}