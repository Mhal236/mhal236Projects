// Name: Muhammad Ali
// Date: 04/19/2022
// Class: CS270
// Assignment: threadpool.c: definition for threadpool.h
// CREDIT: Used hints given through the lecture slides and office hours

#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>

//struct def
typedef struct {
    void (*job)(void *);
    void *arg;
} Task;


Task *task_create(void (*job)(void *), void *arg) {
    Task *task = calloc(1, sizeof(Task));
    //task pointed to job
    task->job = job;
    //task pointed to the argument
    task->arg = arg;
    return task;
}

void *consume(void *arg) {
    threadpool_t *tp = (threadpool_t *) arg;
    while (1) {
        Task *task = pq_next(tp->pq);
        task->job(task->arg);
    }
}

threadpool_t *tp_create(int nthreads) {
    threadpool_t *tp = calloc(1, sizeof(threadpool_t));
    tp->pq = pq_create();
    pthread_t id;
    for (int i = 0; i < nthreads; ++i) {
        pthread_create(&id, NULL, consume, tp);
    }
    return tp;
}

void tp_submit(threadpool_t *tp, void (*job)(void *), void *arg, short prio) {
    pq_insert(tp->pq, task_create(job, arg), prio);
}
