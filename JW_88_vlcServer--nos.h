#include "JW_88_vlcServer.h"

typedef struct {
	char init;
	enum { mNone, mAvail, mClose, mTim, mOff = 0, mReady, mWaitSet, mWaitDel, mWaitReady } workReady, ready, mode;
	short chan;
	pthread_mutex_t mut;
	pthread_cond_t con;
	pthread_cond_t conDel;
	__timerTsk_obj timTsk;
	unsigned int setTim;
}nosDynS;

static void openYtDlF(void* ob) { pipOpen((char*)ob, 1, "youtube-dl"); }
static void killYtD(void* ob) { system("sudo killall youtube-dl"); }
static void killYtDF() { __timerTsk_obj tt; __timerTsk_setTimer(tt, 0, 0, killYtD, NULL); __timerTsk_killTimer(tt, 1); }
static char availF(nosDynS* p);
static void delConF(nosDynS* p, char lock);
static void delaySrcF(void* ob);
static int openCmdF(char* b, short no);
static void nosWorkF(void* X);
static void int2F(void* ob);
static void closeF(nosDynS* p, char x) { p->workReady = mAvail; pthread_mutex_unlock(&p->mut); pthread_cond_signal(&p->con); if (x) killYtD(NULL); else killYtDF(NULL); }
static void closeF2(nosDynS* p, char x) { closeF(p, x); __workerTh_wait(prepWorkObj, prepMainWorker); rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL), sleep(1); }

unsigned long long nosHeapSize() { return sizeof(nosDynS); }
void nosSetObF(__dynHeapP_ptr ptr) { *(nosDynS*)ptr = (nosDynS) { 0, mNone, mNone, mOff, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER }; }
char nosSrcCF(char* buf, int bufL) { return bufL == 11 && buf[5] == 'N' && buf[6] == 'O' && buf[7] == 'S' && isNoF(buf[8]) && isNoF(buf[9]); }