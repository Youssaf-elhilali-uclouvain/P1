#include "my_semaphore.h"

// Déclarations externes des fonctions TTAS
// On utilise les fonctions lock/unlock de ttas_spinlock.c
extern void lock(spinlock_t *s);
extern void unlock(spinlock_t *s);

void my_sem_init(my_semaphore_t *sem, int value) {
    sem->value = value;
    sem->lock.locked = 0;  // Verrou initialement libre
}

void my_sem_wait(my_semaphore_t *sem) {
    // Attente active pure (but pédagogique)
    while (1) {
        lock(&sem->lock);
        
        // Si ressource disponible, on la prend
        if (sem->value > 0) {
            sem->value--;
            unlock(&sem->lock);
            break;  // Ressource acquise !
        }
        

        unlock(&sem->lock);
    }
}

void my_sem_post(my_semaphore_t *sem) {
    lock(&sem->lock);
    sem->value++;  // Libère une ressource
    unlock(&sem->lock);
}
