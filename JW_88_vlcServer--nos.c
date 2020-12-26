#include "JW_88_vlcServer--nos.h"

extern void nosDelPrepF() {
	nosDynS* p = (nosDynS*)execDynHeap;
	pthread_mutex_lock(&p->mut);
	if (p->mode) p->ready = mClose, pthread_mutex_unlock(&p->mut);
	else closeF(p, 0);
}

void nosDelF() {
	nosDynS* p = (nosDynS*)execDynHeap;
	rcPassF((void*)mainBasString[mainBStr_loop], _RCTPCV_detach(rcType_stringOn), NULL);
	pthread_mutex_lock(&p->mut);
	while (p->init == 2) pthread_cond_wait(&p->conDel, &p->mut);
	pthread_mutex_unlock(&p->mut);
	pthread_mutex_destroy(&p->mut);
	pthread_cond_destroy(&p->con);
	pthread_cond_destroy(&p->conDel);
	*((char*)execDynHeap) = '\0';
}

void nosPssF(short no, unsigned int tim) {
	nosDynS* p = (nosDynS*)execDynHeap;
	pthread_mutex_lock(&p->mut); 
	p->chan = no;
	if (!(no = p->init)) p->init = 1, rcPassF((void*)mainBasString[mainBStr_loop], _RCTPCV_detach(rcType_stringOff), NULL);
	if (!p->mode) {
		if (no) closeF2(p, 0);
		else pthread_mutex_unlock(&p->mut);
		p->ready = mNone; p->init = 2, p->mode = mReady, __timerTsk_setTimer(p->timTsk, 1 - !no * 2, 100, delaySrcF, NULL);
	}
	else p->setTim = __timerTsk_timNow(), p->ready = mAvail, pthread_mutex_unlock(&p->mut);
}

static char availF(nosDynS* p) {
	closeF2(p, 1);
	pthread_mutex_lock(&p->mut);
	p->mode = mWaitReady, p->ready = mNone;
	int y = __timerTsk_timeDiff(p->setTim + 100, __timerTsk_timNow());
	if (y <= 0) { pthread_mutex_unlock(&p->mut); delaySrcF(NULL); return 1; }
	else __timerTsk_changeRunSet(p->timTsk, y);
	return 0;
}

static void delConF(nosDynS* p, char lock) {
	if (lock) pthread_mutex_lock(&p->mut);
	p->init = 1; pthread_cond_signal(&p->conDel);
	pthread_mutex_unlock(&p->mut);
}

static void delaySrcF(void* ob) {
	nosDynS* p = (nosDynS*)execDynHeap;
	pthread_mutex_lock(&p->mut);
	switch (p->mode) {

	case mReady: {
		rcStrStr* x = rcStrGet();
		rcPassF((void*)mainBasString[mainBStr_fd0], _RCTPCV_join(rcType_stringAdd), x->b);
		x->l = openCmdF(x->b, p->chan); if (p->ready == mAvail) p->ready = mNone;
		openYtDlF((void*)x->b);
		rcStrRet(x);
		prepWorkOpen(nosWorkF, NULL);
		p->mode = mWaitSet;
	} break;

	case mWaitSet:
		switch (p->ready) {
		case mClose: __timerTsk_killTimer(p->timTsk, 3); closeF(p, 1); delConF(p, 1); return;
		case mAvail: if (availF(p)) return; break;
		case mNone: p->mode = mWaitDel; break;
		} break;

	case mWaitDel:
		switch (p->ready) {
		case mClose: __timerTsk_killTimer(p->timTsk, 3); closeF(p, 1); delConF(p, 1); return;
		case mAvail: if (availF(p)) return; break;
		case mNone: p->mode = mOff, __timerTsk_killTimer(p->timTsk, 3); delConF(p, 0); return;
		} break;

	case mWaitReady: {
		switch (p->ready) {
		case mClose: __timerTsk_killTimer(p->timTsk, 3), delConF(p, 0); return;
		case mNone: p->mode = mReady; pthread_mutex_unlock(&p->mut); __timerTsk_changeRunSet(p->timTsk, 100); delaySrcF(NULL); return;
		case mAvail: {
			p->ready = mNone;
			int y = __timerTsk_timeDiff(p->setTim + 100, __timerTsk_timNow());
			if (y <= 0) { p->mode = mReady; __timerTsk_changeRunSet(p->timTsk, 100); pthread_mutex_unlock(&p->mut); delaySrcF(NULL); return; }
			else __timerTsk_changeRunSet(p->timTsk, y);
		} } break;
	} }
	pthread_mutex_unlock(&p->mut);
}

static int openCmdF(char* b, short no) {
	unsigned int l = charCp(b, "youtube-dl -f 0 -o - \"https://nos.nl/uitzending/");
	l += charCp(b + l, (char*[]) { "nos-journaal", "nos-achtuurjournaal", "nieuwsuur" }[no]);
	return l += charCp(b + l, ".html\"");
}

static void nosWorkF(void* X) {
	nosDynS* p = (nosDynS*)execDynHeap;
	for (;;) {
		char out = 0;
		for (;;) {
			pipForwardVlc(0);
			pthread_mutex_lock(&p->mut);
			if (p->workReady) p->workReady = !(out = 1);
			pthread_mutex_unlock(&p->mut);
			break;
		}
		if (out) break;
		__timerTsk_obj timO;
		char cnt[18]; cnt[16] = cnt[17] = 0;
		__timerTsk_setTimer(timO, -2, 500, int2F, (void*)cnt);
		pthread_mutex_lock(&p->mut);
		while (!p->workReady) pthread_cond_wait(&p->con, &p->mut);
		pthread_mutex_unlock(&p->mut);
		__timerTsk_killTimer(timO, 0);
		pthread_mutex_lock(&p->mut);
		char x = p->workReady;
		p->workReady = mNone;
		if (x == mAvail) { pthread_mutex_unlock(&p->mut); return; }
		if ((x = p->mode) && p->mode != mWaitDel) { pthread_mutex_unlock(&p->mut); return; } 
		p->ready == mAvail && (p->ready = 0), p->mode = p->workReady = mNone;
		if (x) pthread_mutex_unlock(&p->mut), __timerTsk_killTimer(p->timTsk, 1), pthread_mutex_lock(&p->mut);
		p->mode = mWaitSet;
		rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL);
		rcStrStr* X = rcStrGet();
		rcPassF((void*)mainBasString[mainBStr_fd0], _RCTPCV_join(rcType_stringAdd), X->b);
		X->l = openCmdF(X->b, p->chan);
		{
			__timerTsk_obj tt; __timerTsk_setTimer(tt, 0, 0, openYtDlF, (void*)X->b); __timerTsk_killTimer(tt, 1);
		}
		rcStrRet(X); p->init = 2;
		pthread_mutex_unlock(&p->mut);
		__timerTsk_setTimer(p->timTsk, 1, 100, delaySrcF, NULL);
	}
}

static void int2F(void* ob) {
	char* cnt = (char*)ob;
	if (cnt[17] == 2) return;
	if (vlcPauzeF() && !(cnt[17] = 0)) return;
	rcPassF((void*)mainBasString[mainBStr_gtTime], _RCTPCV_join(rcType_string), cnt + cnt[17] * 8);
	if (!cnt[17]) cnt[17] = 1;
	else if (vergCharP(cnt, cnt + 8)) {
		nosDynS* p = (nosDynS*)execDynHeap;
		pthread_mutex_lock(&p->mut);
		if (!p->workReady) p->workReady = mTim, cnt[17] = 2;
		else cnt[17] = 3;
		pthread_mutex_unlock(&p->mut);
		if (cnt[17] == 2) pthread_cond_signal(&p->con); 
		else cnt[17] = 2;
	}
	else cnt[17] = 0;
}