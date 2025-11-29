#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H

#include "spinlock.h"

// Sémaphore maison basé sur TTAS
typedef struct {
    int value;           // Compteur du sémaphore
    spinlock_t lock;     // Verrou TTAS pour protéger 'value'
} my_semaphore_t;

// Initialise le sémaphore avec une valeur initiale
void my_sem_init(my_semaphore_t *sem, int value);

// Équivalent de sem_wait() : décrémente le sémaphore
void my_sem_wait(my_semaphore_t *sem);

// Équivalent de sem_post() : incrémente le sémaphore
void my_sem_post(my_semaphore_t *sem);

#endif
