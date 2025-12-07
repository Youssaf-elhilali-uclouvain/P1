#include "spinlock.h"

static inline int xchg(volatile int *addr, int new_val) {
    int result;
    __asm__ volatile(
        "xchg %0, %1"
        : "=r"(result), "+m"(*addr)
        : "0"(new_val)
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
        
        // si rien, alors on continue dans la boucle
    }
}

void unlock(spinlock_t *s) {
    __asm__ volatile("" ::: "memory");
    s->locked = 0;
}
