#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>

typedef void (*job_f)(_Atomic bool *should_shutdown, void *arg);

typedef struct threadpool threadpool_t;

threadpool_t *thpool_init(int num_threads);
int           thpool_add_work(threadpool_t *pool, job_f function, void *arg);
void          thpool_wait(threadpool_t *pool);
void          thpool_destroy(void *thpool);
void          thpool_pause(threadpool_t *pool);
void          thpool_resume(threadpool_t *pool);
int           thpool_num_threads_working(threadpool_t *pool);

#endif // THREAD_POOL_H
