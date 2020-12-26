#include "JW_88_timerTasks--priv.h"

void __timerTsk_begin() {
	{
		pthread_t t;
		pthread_create(&t, NULL, timeOpen, NULL);
		pthread_detach(t);
	}
	pthread_t t;
	pthread_create(&t, NULL, timeWorkOpen, NULL);
	pthread_detach(t);
}

void __timerTsk_setTimer(__timerTsk_obj obj, char flagTim0Int1ForcedInt2InmeditNEG, unsigned int value, basUniF pFunc, void* pObj) {
	timO* x = (timO*)obj;
	pthread_cond_init(&x->con, NULL);
	x->trigg = timeV;
	x->set = value + (!flagTim0Int1ForcedInt2InmeditNEG && !value);
	x->pF = pFunc;
	x->pO = pObj;
	x->fl = flagTim0Int1ForcedInt2InmeditNEG * (1 - 2 * (flagTim0Int1ForcedInt2InmeditNEG < 0 || (x->trigg += value, 0)));
	pthread_mutex_lock(&mut);
	while (readyCom) pthread_cond_wait(&conWait, &mut);
	pssObj = x;
	readyCom = 1;
	pthread_mutex_unlock(&mut);
	pthread_cond_signal(&con);
}

void __timerTsk_killTimer(__timerTsk_obj obj, char join2run3fromTim) {
	pthread_mutex_lock(&mut);
	if (join2run3fromTim != 3) while (readyCom) pthread_cond_wait(&conWait, &mut);
	timO* x = (timO*)obj;
	if (join2run3fromTim == 3) { pthread_mutex_unlock(&mut); pthread_cond_destroy(&x->con); x->fl = -1; return; }
	else if (!join2run3fromTim || ((join2run3fromTim != 2) && x->set)) {
		pssObj = x;
		readyCom = 2 + (join2run3fromTim == 1);
		pthread_mutex_unlock(&mut);
		pthread_cond_signal(&con);
		pthread_mutex_lock(&mut);
	}
	else if (join2run3fromTim > 1) {
		x->fl = 0;
		pthread_mutex_unlock(&mut);
		pthread_cond_destroy(&x->con);
		return;
	}
	while (x->fl || x->set) pthread_cond_wait(&x->con, &mut);
	pthread_mutex_unlock(&mut);
	pthread_cond_destroy(&x->con);
}

int __timerTsk_timeDiff(unsigned int x, unsigned int y) {
	if (y < x) x -= y;
	else x += 1 + ((unsigned int)(-1) - y);
	if (y = x > (unsigned int)(-1) / 2) x = ((unsigned int)(-1) - x) + 1;
	return x * (1 - (y * 2));
}

static void* timeOpen() {
	for (;; usleep(10000), timeV++)
		if (triggSet && __timerTsk_timeDiff(triggTim, timeV) <= 0) {
			pthread_mutex_lock(&mutTime); char x;
			if (x = (triggSet && __timerTsk_timeDiff(triggTim, timeV) <= 0)) triggSet = 0, readyTim = 1;
			pthread_mutex_unlock(&mutTime);
			if (x) pthread_cond_signal(&con);
		}
}

static void* timeWorkOpen() {
	static short cnt = 0;
	for (;;) {
		pthread_mutex_lock(&mut);
		char x;
		while (!readyCom && !readyTimF(0)) pthread_cond_wait(&con, &mut);
		switch (readyCom) {
		case 0: timFunc(&cnt); break;
		case 1: newTim(pssOF(), cnt++); break;
		case 2: cnt -= delTime(pssOF(), cnt); break;
		case 3: pssOF()->fl = 0; break;
		}
	}
}

static void timFunc(short* cnt) {
	pthread_mutex_unlock(&mut);
	pthread_cond_signal(&conWait);
	for (;;) {
		__timerTsk_timeVal = beginObj->trigg;
		if (beginObj->fl || beginObj->set) beginObj->pF(beginObj->pO);
		if (beginObj->fl < 1) {
			beginObj->set = 0;
			if (!beginObj->fl) pthread_cond_signal(&beginObj->con);
			else beginObj->fl = 0;
			if (!--*cnt) return;
			beginObj = beginObj->next;
		}
		else {
			if (beginObj->fl == 1) beginObj->trigg += beginObj->set;
			else beginObj->trigg = timeV + beginObj->set;
			if (*cnt == 1 || __timerTsk_timeDiff(beginObj->trigg, beginObj->next->trigg) < 0) break;
			timO *p = beginObj;
			beginObj = p->next;
			sortTrigg(p, *cnt);
		}
		if (beginObj->trigg != triggTim) break;
	}
	triggSetF();
}

static void newTim(timO* pssO, short cnt) {
	if (!cnt) beginObj = pssO, triggTim = !pssO->trigg;
	else if (__timerTsk_timeDiff(pssObj->trigg, beginObj->trigg) < 0) pssO->next = beginObj, beginObj = pssO;
	else sortTrigg(pssO, cnt + 1);
	if (triggTim != beginObj->trigg) triggSetF();
}

static char delTime(timO* P, short cnt) {
	if (!P->set && !P->fl) return 0;
	char no = 0;
	timO* p = beginObj;
	if (--cnt) {
		if (beginObj == P) beginObj = p = P->next;
		else if (cnt == 1) beginObj->next = NULL;
		else if (!P->next) toObj(cnt - 1)->next = NULL, no = cnt;
		else for (short tt = 1; (p->next != P || (no = tt, 0)) || (p = p->next = P->next, 0); tt++, p++);
		triggSetF();
	}
	pthread_mutex_lock(&mut);
	P->set = P->fl = 0;
	pthread_cond_signal(&P->con);
	pthread_mutex_unlock(&mut);
	return 1;
}

static void triggSetF() {
	pthread_mutex_lock(&mutTime);
	triggSet = !(readyTim = __timerTsk_timeDiff(triggTim = beginObj->trigg, timeV) <= 0);
	pthread_mutex_unlock(&mutTime);
}

static void sortTrigg(timO* x, short cnt) {
	timO* p = beginObj;
	for (short tt = 2, t = x->trigg; tt < cnt && __timerTsk_timeDiff(t, p->next->trigg) >= 0; tt++, p = p->next);
	p->next = (x->next = p->next, x);
}

static timO* pssOF() {
	timO* x = pssObj;
	readyCom = 0;
	pthread_mutex_unlock(&mut);
	pthread_cond_signal(&conWait);
	return x;
}