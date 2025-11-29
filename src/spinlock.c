#include "spinlock.h"

static inline int xchg(volatile int addr, int newval) {
    int result;
    asm volatile(
        "xchg %0, %1"
        : "=r"(result), "+m"(addr)
        : "0"(newval)
        : "memory"
    );
    return result;
}

void lock(spinlock_t s) {
    while (xchg(&s->locked, 1) == 1) {
        // attente active
    }
}

void unlock(spinlock_ts) {
    asm volatile("" ::: "memory");
    s->locked = 0;
}