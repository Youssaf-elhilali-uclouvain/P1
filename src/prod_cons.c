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
int N_PROD;
int N_CONS;

void* producteur(void* arg) {
    long id = (long)arg;
    int nb_items = 131072 / N_PROD;
    
    for (int i = 0; i < nb_items; i++) {
        // Traitement (en dehors de la zone critique)
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
    int nb_items = 131072 / N_CONS;
    
    for (int i = 0; i < nb_items; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Traitement (en dehors de la zone critique)
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

    pthread_t *producteurs = malloc(N_PROD * sizeof(pthread_t));
    pthread_t *consommateurs = malloc(N_CONS * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < N_PROD; i++) {
        pthread_create(&producteurs[i], NULL, producteur, (void*)i);
    }

    for (long i = 0; i < N_CONS; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, (void*)i);
    }

    for (int i = 0; i < N_PROD; i++) {
        pthread_join(producteurs[i], NULL);
    }

    for (int i = 0; i < N_CONS; i++) {
        pthread_join(consommateurs[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(producteurs);
    free(consommateurs);

    double elapsed_sec = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed_sec);

    return 0;
}