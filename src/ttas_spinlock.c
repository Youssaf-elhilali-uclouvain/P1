#include "spinlock.h"

// Instruction atomique test-and-set avec xchg
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

// Algorithme test-and-test-and-set (TTAS)
void lock(spinlock_t *s) {
    while (1) {
        // Premier TEST : lecture simple (non-atomique)
        // Boucle passive tant que le verrou est occupé
        while (s->locked == 1) {
            // Attente active avec lecture depuis le cache local
            // Pas d'opération atomique coûteuse ici !
        }
        
        // Deuxième TEST-AND-SET : opération atomique
        // Seulement si le verrou semble libre
        if (xchg(&s->locked, 1) == 0) {
            // On a acquis le verrou !
            break;
        }
        // Sinon, on retourne au premier test
    }
}

void unlock(spinlock_t *s) {
    // Barrière mémoire pour éviter la réorganisation des instructions
    __asm__ volatile("" ::: "memory");
    s->locked = 0;
}
