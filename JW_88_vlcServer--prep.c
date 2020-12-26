#include "JW_88_vlcServer--prep.h"

void prepWorkOpen(basUniF p, void* o) {
	static workO x;
	ready[memWork] = 0; 
	__workerTh_open(prepWorkObj, workF, (x = (workO){p, o}, &x), prepMainWorker);
}

static void readyF() {
	pthread_mutex_lock(&mut);
	for (char u; u = -1; ) {
		while ((++u < 2 || (u = 0)) && !(ready[u] == 1 && (u = 1)));
		if (u) break; pthread_cond_wait(&con, &mut);
	} pthread_mutex_unlock(&mut);
}

static void conF(member x, workO* y, char flagForceW2FrcTke3Bth) {
	pthread_mutex_lock(&mut); 
	if (!flagForceW2FrcTke3Bth || flagForceW2FrcTke3Bth == 3)
		while (ready[x]) 
			pthread_cond_wait(&conW, &mut); 
	if (y) y->p(y->o);
	ready[x] = 1; 
	if (flagForceW2FrcTke3Bth > 2)
		while (ready[x])
			pthread_cond_wait(&conW, &mut);
	pthread_mutex_unlock(&mut); 
	pthread_cond_signal(&con); 
}

char prepFunc(char bufP, int* bufLP, int* bTimP) {
	if (prepCloseV && ready[memWork] == 2) ready[memWork] = 1;
	for (char out = 0;;) {
		readyF();
		for (member m = 0; !out && m < 2; m++) 
			if (ready[m] == 1)
			switch (m) {
			case memNet:
				pthread_mutex_lock(&mut);
				*bufLP = bufP;
				bufP = netBuf;
				netBuf = *bufLP;
				*bufLP = netBufL;
				*bTimP = netBufT;
				ready[memNet] = 0;
				pthread_mutex_unlock(&mut);
				if (vlcSysOffCF(execBufgetP(bufP))) sysCloseF();
				if (!prepCloseV) out = 1;
				break;
			case memWork:
				ready[memWork] = 2;
				if (prepCloseV)
					if (prepCloseV == 1) vlcClearF();
					else prepCloseV = 0, out = 1;
				break;
		} pthread_cond_broadcast(&conW);
		if (out) return bufP;
	}
}

static void sysCloseF() {
	if (vlcMode) 
		!prepCloseV && vlcClosePlayer(),
		__workerTh_wait(prepWorkObj, prepMainWorker),
		vlcClearF();
	rcPassF((void*)mainBasString[mainBStr_shtdwn], _RCTPCV_detach(rcType_string), NULL);
	system("sudo shutdown -h now");
	for (;;) sleep(60);
}

static void pssF(void* o) { 
	struct { char* x; int l; unsigned int t; }*x = o; 
	netBufL = x->l;
	x->l = *x->x;
	*x->x = netBuf;
	netBuf = x->l;
	netBufT = x->t;
}