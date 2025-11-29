#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// --- Constantes du Problème ---
#define BUFFER_SIZE 8
#define TOTAL_ITEMS 131072
// Le travail à simuler (utilisé pour "simuler" un traitement)
#define CPU_WORK 10000

// --- Variables Partagées ---
int buffer[BUFFER_SIZE];
int in = 0;   // Index de la prochaine insertion (Producteur)
int out = 0;  // Index de la prochaine extraction (Consommateur)

pthread_mutex_t buffer_mutex;  // Exclusion mutuelle pour l'accès au buffer (in/out)
sem_t empty;                   // Compte les places vides (initialisé à BUFFER_SIZE)
sem_t full;                    // Compte les places pleines (initialisé à 0)

// Pour s'assurer que l'ensemble du travail est fait.

int items_produced = 0;
int items_consumed = 0;

void busy_wait() {
    for (volatile int i = 0; i < CPU_WORK; i++);
}

void* producer(void* arg) {
    long thread_id = (long)arg;

    while (1) {
        // Vérifier fin AVANT sem_wait
        pthread_mutex_lock(&buffer_mutex);
        if (items_produced >= TOTAL_ITEMS) {
            pthread_mutex_unlock(&buffer_mutex);
            break;
        }
        pthread_mutex_unlock(&buffer_mutex);

        // Attendre place vide
        sem_wait(&empty);

        // Vérifier fin APRÈS sem_wait (obligatoire)
        pthread_mutex_lock(&buffer_mutex);
        if (items_produced >= TOTAL_ITEMS) {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&empty);
            break;
        }

        // Production
        buffer[in] = (int)thread_id;
        in = (in + 1) % BUFFER_SIZE;
        items_produced++;

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full);

        busy_wait();
    }
    return NULL;
}


void* consumer(void* arg) {
    long thread_id = (long)arg;

    while (1) {
        // Vérifier fin AVANT sem_wait
        pthread_mutex_lock(&buffer_mutex);
        if (items_consumed >= TOTAL_ITEMS) {
            pthread_mutex_unlock(&buffer_mutex);
            break;
        }
        pthread_mutex_unlock(&buffer_mutex);

        sem_wait(&full);

        // Vérifier fin APRÈS sem_wait
        pthread_mutex_lock(&buffer_mutex);
        if (items_consumed >= TOTAL_ITEMS) {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&full);
            break;
        }

        int data = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        items_consumed++;

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty);

        busy_wait();
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Nb_Producteurs> <Nb_Consommateurs>\n", argv[0]);
        return 1;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    if (num_producers <= 0 || num_consumers <= 0) {
        fprintf(stderr, "Erreur: Le nombre de threads doit être > 0.\n");
        return 1;
    }

    pthread_t *prod_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t *cons_threads = malloc(num_consumers * sizeof(pthread_t));
    
    if (!prod_threads || !cons_threads) {
        perror("malloc");
        return 1;
    }

    pthread_mutex_init(&buffer_mutex, NULL);
    // Le sémaphore 'empty' est initialisé à la taille du buffer (8)
    sem_init(&empty, 0, BUFFER_SIZE); 
    // Le sémaphore 'full' est initialisé à 0
    sem_init(&full, 0, 0); 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long i = 0; i < num_producers; i++) {
        pthread_create(&prod_threads[i], NULL, producer, (void*)i);
    }

    for (long i = 0; i < num_consumers; i++) {
        pthread_create(&cons_threads[i], NULL, consumer, (void*)i);
    }
    
    for (int i = 0; i < num_producers; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(prod_threads);
    free(cons_threads);

    // Affichage du temps
    double elapsed_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed_sec);

    return 0;
}