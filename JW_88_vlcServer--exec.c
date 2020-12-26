#include "JW_88_vlcServer--exec.h"

static void* open() { 
	int bTim, bufL;
	for (char x = 0;;) x = prepFunc(x, &bufL, &bTim), func(execBufgetP(x), bufL, bTim);
}

static void beginF() {
	rcStrStr* x = rcStrGet();
	for (char t = 0; t < 2; t++)
		rcPassF((void*)(unsigned long long)(0), _RCTPCV_join(rcType_dvb), x->b), sleep(20),
		rcPassF((void*)mainBasString[mainBStr_clear], _RCTPCV_join(rcType_string), x->b), sleep(10);
	rcStrRet(x);
}

static void func(char* buf, int bufL, int funcTime) {
	if (vlcOffCF(buf) && (!vlcMode || vlcClosePlayer())) return;
	if (vlcSrcCF(buf, bufL) && vlcSrcPssF(buf, bufL, funcTime)) return;
}