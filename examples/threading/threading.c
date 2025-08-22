#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data *thread_data_args = (struct thread_data *) thread_param;
    int ret;
    
    DEBUG_LOG("Sleep for %dms...\n", thread_data_args->wait_to_obtain_ms);
    usleep(thread_data_args->wait_to_obtain_ms*1000);

    DEBUG_LOG("Waked up, locking the mutex...\n");
    ret = pthread_mutex_lock(thread_data_args->mutex);
    if (ret < 0) {
        DEBUG_LOG("Lock the mutex failed with ret=%d\n", ret);
        pthread_exit(&ret);
    }
    
    DEBUG_LOG("Sleep for %dms...\n", thread_data_args->wait_to_release_ms);
    usleep(thread_data_args->wait_to_release_ms*1000);

    DEBUG_LOG("Waked up, unlocking the mutex...\n");
    ret = pthread_mutex_unlock(thread_data_args->mutex);
    if (ret < 0) {
        DEBUG_LOG("Unlock the mutex failed with ret=%d\n", ret);
        pthread_exit(&ret);
    } else {
        thread_data_args->thread_complete_success = true;
        // pthread_exit()
    }
    
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    int ret;

    struct thread_data *thread_data_parmas = malloc(sizeof(struct thread_data));

    thread_data_parmas->mutex = mutex;
    thread_data_parmas->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_data_parmas->wait_to_release_ms = wait_to_release_ms;
    thread_data_parmas->thread_complete_success = false;

    ret = pthread_create(thread, NULL, threadfunc, thread_data_parmas);
    if (ret < 0) {
        ERROR_LOG("Create thread error with ret = %d\n", ret);
        return false;
    } else {
        DEBUG_LOG("Create thread successfully with thread_id = %lu\n", *thread);
    }

    return true;
}

