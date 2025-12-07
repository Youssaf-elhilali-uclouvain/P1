#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

pthread_mutex_t mutex;
sem_t db;
int nb_lectures = 0;
int N_LECTEURS;
int N_ECRIVAINS;

void* lecteur() {
    int nb = 25600 / N_LECTEURS;
    
    for (int i = 0; i < nb; i++) {
        pthread_mutex_lock(&mutex);
        nb_lectures++;
        if (nb_lectures == 1) {
            sem_wait(&db);
        }
        pthread_mutex_unlock(&mutex);

        // lecture
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
    int nb = 6400 / N_ECRIVAINS;
    
    for (int i = 0; i < nb; i++) {
        sem_wait(&db);

        // ecriture
        for (int j = 0; j < 10000; j++);

        sem_post(&db);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N_Lecteurs> <N_Ecrivains>\n", argv[0]);
        return 1;
    }

    N_LECTEURS = atoi(argv[1]);
    N_ECRIVAINS = atoi(argv[2]);

    pthread_t *threads_lect = malloc(N_LECTEURS * sizeof(pthread_t));
    pthread_t *threads_ecr = malloc(N_ECRIVAINS * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&db, 0, 1);

    struct timespec debut,fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (long i = 0; i < N_LECTEURS; i++) {
        pthread_create(&threads_lect[i], NULL, lecteur, (void*)i);
    }

    for (long i = 0; i < N_ECRIVAINS; i++) {
        pthread_create(&threads_ecr[i], NULL, ecrivain, (void*)i);
    }

    for (int i = 0; i < N_LECTEURS; i++) {
        pthread_join(threads_lect[i], NULL);
    }

    for (int i = 0; i < N_ECRIVAINS; i++) {
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
