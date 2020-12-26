#include "JW_88_vlcServer--vlc.h"

static void srcWrkF(void* o) {
	unsigned int tim;
	int tD;
	pthread_mutex_lock(&srcSetMut);
	if (retF()) return;
	srcState = srcSet != 1, srcSet = 1, tim = srcSetTime;
	if (!init) init = 1;
	else rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL), sleep(1);
	if (!srcState) srcPssTypeF();
	else pthread_mutex_unlock(&srcSetMut);
	for (;;) {
		if ((tD = __timerTsk_timeDiff(tim + 100, __timerTsk_timNow())) && tD > 0) sleep(tD / 100), usleep((tD % 100) * 10000);
		pthread_mutex_lock(&srcSetMut);
		if (retF()) return;
		if (!srcState)
			if (srcSet == 2) srcState = 1;
			else {
				srcState = 1; 
				(srcSet != -1 && (tD = 100)) || (tD = __timerTsk_timeDiff(srcSetTime + 100, __timerTsk_timNow()));
				pthread_mutex_unlock(&srcSetMut); 
				sleep(tD / 100), usleep((tD % 100) * 10000);
				pthread_mutex_lock(&srcSetMut); 
				if (retF()) return;
				srcSet *= (srcSet == 2) * 3 - 1;
				if (retF()) return;
			}
		if (srcSet == 2) {
			rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL), sleep(1), srcSet = 1, tim = srcSetTime, pthread_mutex_unlock(&srcSetMut);
			continue;
		}
		srcSet = 3, srcPssTypeF(), sleep(1), pthread_mutex_lock(&srcSetMut);
		if ((srcSet == 3 && !(srcSet = -1)) || retF()) return;
		rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL), sleep(1), srcSet = 1, tim = srcSetTime, pthread_mutex_unlock(&srcSetMut);
	}
}

char vlcClosePlayer() {
	switch (vlcMode) {
	case vlcMod_src: srcClose(); break;
	case vlcMod_nos: nosDelPrepF(); break;
	} return prepCloseV = 1;
}

void vlcClearF() {
	switch (vlcMode) {
	case vlcMod_src: init = 0; break;
	case vlcMod_nos: nosDelF(); break;
	} rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_detach(rcType_string), NULL);
	prepWorkOpen(sleepOne, NULL);
	vlcPauze = vlcMode = vlcMod_off; prepCloseV = 2;
}

static void srcClose() {
	pthread_mutex_lock(&srcSetMut);
	char x = srcSet > 2 || (srcSet && !(srcState % 2));
	if (srcSet) srcSet = -1, srcSetTime = __timerTsk_timNow();
	pthread_mutex_unlock(&srcSetMut);
}

static vlcModEn srcModF() {
	if (nosSrcCF(source, sourceL)) return vlcMod_nos;
	return vlcMod_src;
}

static void srcPssTypeF() {
	void *arg; rcTypeEn type;
	if (dvbSrcCF(source, sourceL))
		type = _RCTPCV_detach(rcType_dvb), arg = (void*)(unsigned long long)((source[9] - '0') * 100 + (source[10] - '0') * 10 + (source[11] - '0'));
	else type = _RCTPCV_detach(rcType_rcStr), arg = (void*)rcStrGet(), ((rcStrStr*)arg)->l = rcAddcp(((rcStrStr*)arg)->b, source + 5) - 1;
	pthread_mutex_unlock(&srcSetMut);
	rcPassF(arg, type, NULL);
}

char vlcSrcPssF(char* buf, int bufL, unsigned int tim) {
	int x;
	if (!((sourceL == bufL && vergCharP(buf, source) && !(buf[0] = '\0') && !vlcMode) || (buf[0] && (sourceL = charCp(source, buf))))) return 1;
	if ((x = srcModF()) != vlcMode && vlcMode) return vlcClosePlayer();
	switch (!vlcMode && (vlcMode = x), vlcMode) {
	
	case vlcMod_src: {
		pthread_mutex_lock(&srcSetMut);
		signed char yy;
		if (srcSet == -1) yy = srcSet = 2;
		else if ((yy = srcSet) % 2 || (!srcSet && (srcState = 2))) srcSet++;
		srcSetTime = tim;
		pthread_mutex_unlock(&srcSetMut);
		if (!yy) prepWorkOpen(srcWrkF, NULL);
	} break;
	
	case vlcMod_nos:
		__dynHeapP_chckOpenF(execDynHeap, nosSetObF);
		nosPssF((buf[8] - '0') * 10 + (buf[9] - '0'), tim);
		break;
	}
	return 1;
}

char vlcPauzeF() { 
	char x; 
	if ((x = vlcPauze) < 2) return x; 
	pthread_mutex_lock(&vlcPauzeMut);
	vlcPauze = vlcPauze == 3; 
	pthread_mutex_unlock(&vlcPauzeMut); 
	return 1; 
}

char vlcPauzeSetF(char x) { 
	pthread_mutex_lock(&vlcPauzeMut); 
	if (x) vlcPauze = 3; 
	else if (vlcPauze != 1) vlcPauze = 4;
	else vlcPauze = 0;
	pthread_mutex_unlock(&vlcPauzeMut); }