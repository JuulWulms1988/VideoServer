#include "JW_88_pthreadShared.h"
#include <stdio.h>

typedef struct shareS {
	pthread_mutex_t mut;
	pthread_mutex_t mut0;
	short connec;
	short connecR;
	pthread_cond_t con;
	pthread_cond_t con0;
}shareS;

void pthread_sharedMutex_init(pthread_sharedMutex_t p) { 
	shareS* x = (shareS*)p;
	pthread_mutex_init(&x->mut, NULL); 
	pthread_mutex_init(&x->mut0, NULL); 
	x->connec = x->connecR = 1;
	pthread_cond_init(&x->con, NULL);
}

void pthread_sharedMutex_destroy(pthread_sharedMutex_t p) {
	shareS* x = (shareS*)p;
	pthread_mutex_destroy(&x->mut);
	pthread_mutex_destroy(&x->mut0);
	pthread_cond_destroy(&x->con);
}

void pthread_sharedMutex_lock(pthread_sharedMutex_t p) {
	shareS* x = (shareS*)p;
	pthread_mutex_lock(&x->mut); 
	while (x->connec < 0) 
		pthread_cond_wait(&x->con, &x->mut);
	short y = x->connec;
	x->connec *= -1;
	pthread_mutex_unlock(&x->mut);
	pthread_mutex_lock(&x->mut0); 
	while (x->connecR != y) 
		pthread_cond_wait(&x->con0, &x->mut0); 
}

void pthread_sharedMutex_unlock(pthread_sharedMutex_t p) {
	shareS* x = (shareS*)p;
	x->connecR = x->connec = 1;
	pthread_mutex_unlock(&x->mut0);
	pthread_cond_signal(&x->con);
}

void pthread_sharedMutex_lock_shared(pthread_sharedMutex_t p) {
	shareS* x = (shareS*)p;
	pthread_mutex_lock(&x->mut); 
	while (x->connec < 0) 
		pthread_cond_wait(&x->con, &x->mut); 
	x->connec = (x->connec % 32766) + 1;
	pthread_mutex_unlock(&x->mut);
}

void pthread_sharedMutex_unlock_shared(pthread_sharedMutex_t p) {
	shareS* x = (shareS*)p;
	pthread_mutex_lock(&x->mut0);
	x->connecR = (x->connecR % 32766) + 1;
	char fl = x->connec < 0 && (-1) * x->connec == x->connecR;
	pthread_mutex_unlock(&x->mut0);
	if (fl) pthread_cond_signal(&x->con0);
}