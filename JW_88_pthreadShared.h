#ifndef JW_88_pthreadShared_H_   
#define JW_88_pthreadShared_H_

#include <pthread.h>

typedef char pthread_sharedMutex_t[2 * sizeof(short) + 2 * sizeof(pthread_mutex_t) + 2 * sizeof(pthread_cond_t)];

extern void pthread_sharedMutex_init(pthread_sharedMutex_t p);
extern void pthread_sharedMutex_destroy(pthread_sharedMutex_t p);
extern void pthread_sharedMutex_lock(pthread_sharedMutex_t p);
extern void pthread_sharedMutex_unlock(pthread_sharedMutex_t p);
extern void pthread_sharedMutex_lock_shared(pthread_sharedMutex_t p);
extern void pthread_sharedMutex_unlock_shared(pthread_sharedMutex_t p);

#endif