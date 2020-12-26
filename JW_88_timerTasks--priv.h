#include "JW_88_timerTasks.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct timO {
	basUniF pF;
	void* pO;
	char fl;
	struct timO* next;
	volatile unsigned int set;
	unsigned int trigg;
	pthread_cond_t con;
} timO;

static char readyTim = 0, readyCom = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER, mutTime = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t con = PTHREAD_COND_INITIALIZER, conWait = PTHREAD_COND_INITIALIZER;
static volatile unsigned int timeV = 0, triggTim;
static volatile char triggSet = 0;
static timO* pssObj, *beginObj;

static void* timeWorkOpen();
static void* timeOpen();
static void timFunc(short* cnt);
static void newTim(timO* P, short cnt);
static char delTime(timO* P, short cnt);
static void triggSetF();
static void sortTrigg(timO* x, short cnt);
static timO* pssOF();

static void endTimF() { pthread_mutex_lock(&mut); readyCom = 0; pthread_mutex_unlock(&mut); pthread_cond_signal(&conWait); }
static char readyTimF(char x) { return pthread_mutex_lock(&mutTime), x = readyTim && (readyTim = x, 1), pthread_mutex_unlock(&mutTime), x; }
static timO* toObj(short x) { timO* p = beginObj; while (x--) p = p->next; return p; }

unsigned int __timerTsk_timeVal;

void __timerTsk_changeRunSet(__timerTsk_obj obj, short set) { ((timO*)obj)->set = set; }
unsigned int __timerTsk_timNow() { return timeV; }