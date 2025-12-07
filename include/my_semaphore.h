#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H

#include "spinlock.h"

// Sémaphore maison (Test-and-test-and-set)
typedef struct {
    int value;           
    spinlock_t lock;     
} my_semaphore_t;

void my_sem_init(my_semaphore_t *sem, int value);

void my_sem_wait(my_semaphore_t *sem);

void my_sem_post(my_semaphore_t *sem);

#endif
