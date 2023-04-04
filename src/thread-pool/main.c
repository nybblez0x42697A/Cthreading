#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "./include/thread_pool.h"
#include "./lib/Signals/include/signal_handlers.h"

threadpool_t * temp_pool = NULL;

void
print_hello(_Atomic bool * should_shutdown, void * arg)
{
    int id             = *((int *)arg);
    int sleep_duration = rand() % 5 + 1;
    printf("Thread %d: Hello, I will sleep for %d seconds.\n",
           id,
           sleep_duration);
    for (int i = 0; i < sleep_duration; i++)
    {
        if (*should_shutdown)
        {
            printf("Thread %d: Terminating early due to shutdown request.\n",
                   id);
            return;
        }
        sleep(1);
    }
    printf("Thread %d: Goodbye!\n", id);
}

void
main_cleanup()
{
    if (temp_pool)
    {
        thpool_pause(temp_pool);
        pthread_mutex_lock(&cleanup_mutex);
        while (thpool_num_threads_working(temp_pool) != 0)
        {
            pthread_cond_wait(&cleanup_cond, &cleanup_mutex);
        }
        pthread_mutex_unlock(&cleanup_mutex);
        thpool_destroy(temp_pool);
    }
    for (int count = 0; count < clean.num_items; count++)
    {
        if (clean.items)
        {
            free(clean.items[count]);
        }
    }
    // free(clean.items);
}

int
main()
{

    int   num_threads = 5;
    int   num_jobs    = 10;
    int * job_ids     = calloc(num_jobs, sizeof(int));
    clean.items       = calloc(2, sizeof(void **));
    if (!job_ids)
    {
        perror("Failed to allocate memory.");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0; i < num_jobs; i++)
    {
        job_ids[i] = i;
    }
    clean.items[0] = job_ids;
    clean.num_items += 1;

    printf("Initializing thread pool with %d threads...\n", num_threads);
    threadpool_t * pool = thpool_init(num_threads);
    temp_pool = pool;
    clean.cleanup_f = main_cleanup;

    if (!pool)
    {
        perror("Failed to allocate memory for pool");
        for (int count = 0; count < num_jobs; count++)
        {
            main_cleanup(NULL);
        }
        return 1;
    }

    install_default_signal_handlers();

    printf("Adding %d jobs to the thread pool...\n", num_jobs);
    for (int i = 0; i < num_jobs; i++)
    {
        thpool_add_work(pool, print_hello, &job_ids[i]);
    }

    printf("Waiting for jobs to complete...\n");
    thpool_wait(pool);

    printf("Destroying thread pool...\n");
    thpool_destroy(pool);

    printf("Done.\n");

    free(job_ids);

    return 0;
}
