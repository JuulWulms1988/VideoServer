#include "JW_88_vlcServer--netC.h"

void netCBegin() {
	bufPss = execBufgetP(2);
	TCPFunc(&serv, checkBsF, "PS00", 4);
};

static void netCFunc(int sock) {
	unsigned int bufT = __timerTsk_timNow();
	bufPss[bufPssL] = '\0';
	if (checkVlcF()) bufPss = execBufgetP(prepPassF(execBufgetN(bufPss), bufPssL, bufT, 1));
	else if (vlcSysOffCF(bufPss)) {
		close(sock), execBufgetP(prepPassF(execBufgetN(bufPss), 7, bufT, 1));
		for (;;) sleep(100);
	}
	bufPssL = 0;
}

static char checkBsF(int bufL, int sock) {
	if (sock < 0) bufPssL = 0, sock = -(sock - 1);
	char* buf = serv.buf;
	for (int t = 0; t < bufL; t++) 
		if (bufPssL > 998) return 0;
		else if ((bufPss[bufPssL] = buf[t]) != '\n') bufPssL++;
		else netCFunc(sock);
	return 1;
}

static char checkVlcF() {
	if (!vlcSrcCF(bufPss, bufPssL))
		return vlcOffCF(bufPss);
	char x;
	if (x = nosSrcCF(bufPss, bufPssL))
		return (bufPss[8] - '0') * 10 + (bufPss[9] - '0') < 3;
	if (x = dvbSrcCF(bufPss, bufPssL))
		return x - 1;
	return 1;
}