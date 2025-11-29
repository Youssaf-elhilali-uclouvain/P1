#include "spinlock.h"

static inline int xchg(volatile int *addr, int newval) {
    int result;
    __asm__ volatile(
        "xchg %0, %1"
        : "=r"(result), "+m"(*addr)
        : "0"(newval)
        : "memory"
    );
    return result;
}

// test-and-test-and-set (TTAS)
void lock(spinlock_t *s) {
    while (1) {
        // Premier test : lecture simple 
        // Boucle tant que le verrou est occupé
        while (s->locked == 1) {
            // Attente 
        }

        // Deuxième test-and-set : opération atomique
        if (xchg(&s->locked, 1) == 0) {
            // On a le verrou
            break;
        }
        // retour à la première étape
    }
}

void unlock(spinlock_t *s) {
    // Barrière mémoire pour éviter la réorganisation des instructions
    __asm__ volatile("" ::: "memory");
    s->locked = 0;
}
