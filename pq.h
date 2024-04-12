#ifndef PQ_H
#define PQ_H

#include <pthread.h>

/* Interface to a very simple *unbounded* thread-safe priority queue.
 * Priority is a signed number; items enqueued with higher prio values are
 * returned before those with lower values. (Negative values are fine.)
 * Items with the same priority are  returned in the order of insertion (FIFO).
 * There is no safe way to destroy a priority queue once it is created,
 * so THIS CODE IS NOT FOR PRODUCTION USE.
 */

typedef struct Node {
    void *item;
    short prio;
    struct Node *next;
} Node;

typedef struct {
    struct Node head;
    pthread_mutex_t mut;
    pthread_cond_t is_empty;
} pq_t;

// create and initialize a prio queue instance. Return NULL on error.
pq_t *pq_create(void);

/* This operation never blocks.  Fails (aborts) if OS runs out of memory.
 * N.B. the second argument is the "value" to be stored at the given prio.
 * It will eventually be returned by pq_next() unless it never becomes the
 * highest priority item.  It can be a pointer to anything, or even
 * an integer type.
 */
void pq_insert(pq_t *q, void *item, short prio);

/* Return the oldest item with highest priority.
 * In other words, the item returns satisfies the predicate:
 * No item in the queue has higher priority AND any item with the same
 * priority was inserted after this item.
 * The calling thread will block if the queue is empty.
 */
void *pq_next(pq_t *q); // returns the oldest item with highest-priority
#endif
