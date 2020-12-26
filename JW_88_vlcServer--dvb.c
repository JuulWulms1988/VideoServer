#include "JW_88_vlcServer--dvb.h"

char dvbSrcCF(char* buf, int bufL) { return ((bufL = bufL == 13) && buf[5] == 'd' && buf[6] == 'v' && buf[7] == 'b' && buf[8] == ':' && isNoF(buf[9]) && isNoF(buf[10]) && isNoF(buf[11])) * (1 + (bufL && (buf[9] - '0') * 100 + (buf[10] - '0') * 10 + (buf[11] - '0') < DVB_CHAN_CNT)); }

int dvbStrF(char* b, int c) {
	struct dvbTvChanS* p = dvbTvChan + c;
	int l = rcAddcp(b, "dvb-c://frequency=");
	b[l++] = p->freq / 100 + '0';
	b[l++] = p->freq / 10 % 10 + '0';
	b[l++] = p->freq % 10 + '0';
	for (char tt = 0; tt < 6; tt++) b[l++] = '0';
	l += charCp(b + l, " :dvb-srate=6900000 :dvb-modulation=64QAM :program=");
	if (p->chan / 10000) b[l++] = p->chan / 10000 + '0';
	b[l++] = p->chan / 1000 % 10 + '0';
	b[l++] = p->chan / 100 % 10 + '0';
	b[l++] = p->chan / 10 % 10 + '0';
	b[l++] = p->chan % 10 + '0';
	return l;
}