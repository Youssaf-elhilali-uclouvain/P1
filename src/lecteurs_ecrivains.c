#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

pthread_mutex_t mutex;
sem_t db;
int nb_lectures = 0;
int Nbre_lecteurs;
int Nbre_ecrivains;

void* lecteur() {
    int nb = 25600 / Nbre_lecteurs;

    for (int i = 0; i < nb; i++) {
        pthread_mutex_lock(&mutex);
        nb_lectures++;
        if (nb_lectures == 1) {
            sem_wait(&db);
        }
        pthread_mutex_unlock(&mutex);

        // on lit
        for (int j = 0; j < 10000; j++);

        pthread_mutex_lock(&mutex);
        nb_lectures--;
        if (nb_lectures == 0) {
            sem_post(&db);
        }
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

void* ecrivain() {
    int nb = 6400 / Nbre_ecrivains;
    
    for (int i = 0; i < nb; i++) {
        sem_wait(&db);

        // on écrit
        for (int j = 0; j < 10000; j++);

        sem_post(&db);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Nbre_lecteurs> <Nbre_ecrivains>\n", argv[0]);
        return 1;
    }

    Nbre_lecteurs = atoi(argv[1]);
    Nbre_ecrivains = atoi(argv[2]);

    pthread_t *threads_lect = malloc(Nbre_lecteurs * sizeof(pthread_t));
    pthread_t *threads_ecr = malloc(Nbre_ecrivains * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&db, 0, 1);

    struct timespec debut,fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < Nbre_lecteurs; i++) {
        pthread_create(&threads_lect[i], NULL, lecteur, (void*)i);
    }

    for (long i = 0; i < Nbre_ecrivains; i++) {
        pthread_create(&threads_ecr[i], NULL, ecrivain, (void*)i);
    }

    for (int i = 0; i < Nbre_lecteurs; i++) {
        pthread_join(threads_lect[i], NULL);
    }

    for (int i = 0; i < Nbre_ecrivains; i++) {
        pthread_join(threads_ecr[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&db);
    free(threads_lect);
    free(threads_ecr);

    double temps = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    printf("%.6f\n", temps);

    return 0;
}
