#ifndef SPINLOCK_H
#define SPINLOCK_H

typedef struct {
    volatile int locked;
} spinlock_t;

void lock(spinlock_t *s);
void unlock(spinlock_t *s);

#endif
