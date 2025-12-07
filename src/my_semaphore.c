#include "my_semaphore.h"

extern void lock(spinlock_t *s);
extern void unlock(spinlock_t *s);

void my_sem_init(my_semaphore_t *sem, int value){
    sem->value = value;
    sem->lock.locked = 0;
}

void my_sem_wait(my_semaphore_t *sem){
    while (1) {
        lock(&sem->lock);
        if (sem->value > 0) {
            sem->value--;
            unlock(&sem->lock);
            break;
        }
        unlock(&sem->lock);
    }
}

void my_sem_post(my_semaphore_t *sem){
    lock(&sem->lock);
    sem->value++;
    unlock(&sem->lock);
}
