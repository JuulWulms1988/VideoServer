#include "JW_88_workerThreads.h"
#include "JW_88_pthreadShared.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE__TH_AR 10

typedef struct {
	char trace;
	short cnt;
	short tot;
	short cat;
	short now;
	struct pssObS {
		short cnt;
		pthread_mutex_t mut;
		pthread_cond_t con;
		pthread_cond_t con0;
		short ready;
		void* ob;
		basUniF p;
	}*pssOb;
	struct obS {
		pthread_cond_t con;
		char ready;
	}*ob;
	pthread_cond_t retCon;
	pthread_cond_t openCon;
	pthread_mutex_t mut;
} wTrS;

typedef struct {
	pthread_mutex_t mut;
	pthread_cond_t con;
	char ready;
	basUniF p;
	void* o;
}obS2;

static void* open(void* arg);
static void* run(void* arg);
static wTrS* pointF(char act, short var);
static short openPrep(void ** arg);
static void __workerTh_waitF(wTrS* p, __workerTh_t WT_pId);

void __workerTh_wait(__workerTh_ point, __workerTh_t WT_pId) { __workerTh_waitF(pointF(0, point), WT_pId); }