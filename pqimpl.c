// Name: Muhammad Ali
// Date: 04/19/2022
// Class: CS270
// Assignment: pqimpl.c: definition for pq.h
// CREDIT: Used hints given through the lecture slides and office hours

#include "pq.h"
#include <stdio.h>
#include <stdlib.h>

//struc
Node *node_create(void *item, short prio, Node *next) {
    Node *node = calloc(1, sizeof(Node));
    node->item = item;
    node->prio = prio;
    node->next = next;
    return node;
}

//def for create
pq_t *pq_create(void) {
    pq_t *q = calloc(1, sizeof(pq_t));
    pthread_mutex_init(&q->mut, NULL);
    pthread_cond_init(&q->is_empty, NULL);
    return q;
}


//def for insert
void pq_insert(pq_t *q, void *item, short prio) {
    pthread_mutex_lock(&q->mut);
    Node *p = &q->head;
    if (p->next == NULL) {
        pthread_cond_signal(&q->is_empty);
    }
    while (p->next != NULL && prio <= p->next->prio) {
        p = p->next;
    }
    p->next = node_create(item, prio, p->next);
    pthread_mutex_unlock(&q->mut);
}

//def for next
void *pq_next(pq_t *q) {
    pthread_mutex_lock(&q->mut);
    while (q->head.next == NULL) {
        pthread_cond_wait(&q->is_empty, &q->mut);
    }
    void *item = q->head.next->item;
    q->head.next = q->head.next->next;
    pthread_mutex_unlock(&q->mut);
    return item;
}
