#include "spinlock.h"

static inline int xchg(volatile int *addr, int newval){
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
        // test 1
        while (s->locked == 1) {
        }
        // test 2
        if (xchg(&s->locked, 1) == 0) {
            break;
        }
    }
}

void unlock(spinlock_t *s){
    __asm__ volatile("" ::: "memory");
    s->locked = 0;
}
