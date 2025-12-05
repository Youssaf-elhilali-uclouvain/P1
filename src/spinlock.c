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

void lock(spinlock_t *s){
    while (1) {
       
        int previous = xchg(&s->locked, 1);
 
        if (previous == 0) {
            // On sort de la boucle, on a le verrou
            break;
        }
        
        // Sinon on continue la boucle pour réessayer
    }
}

void unlock(spinlock_t *s) {
    __asm__ volatile("" ::: "memory");
    s->locked = 0;
}
