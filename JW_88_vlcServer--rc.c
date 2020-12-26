#include "JW_88_vlcServer--rc.h"

void rcBegin() {
	{
		pthread_t p;
		pthread_create(&p, NULL, open, NULL);
		pthread_detach(p);
	}
	waitRepl();
}

static void* open() {
	{
		rcStrStr* x = rcStrGet();
		charCp(x->b, "cvlc -I rc --rc-host=:10000 --no-sout-all --sout=\'#duplicate{dst=std{access=http{user=X,pwd=X},mux=ts,dst=:8080/stream},dst=udp{dst=192.168.180.255,port=1234,mux=ts}}\' --sout-mux-caching=250 --no-sout-display --sout-keep --loop");
		pipOpen(x->b, 0, "vlc");
		rcStrRet(x);
	}
	TCPFunc(&serv, netFunc, NULL, 0);
}

static char netFunc(int bufL, int sock) {
	static char reset = 0, flagS = 0;
	if (sock < 0) reset = 0, readStream(0), readB[readL = 0] = '\0', sock = -(sock - 1);
	if (readStream(bufL)) return 1;
	if (!reset || (!(reset = 0) && !passType)) get(), !(passType % 2) && (readB[readL = 0] = '\0');
	rcStrStr* p;
	passType *= prep(&p);
	p->b[p->l] = '\0';
	if (TCPSend(sock, p->b, p->l, 0) < 0 && (reset = 1) && (passType *= 1 - 2 * (passType < 1))) { rcStrRet(p); return 0; }
	rcStrRet(p);
	passType *= 1 - (passType < 0); 
	return 1;
}

int rcPassF(void* passV, rcTypeEn passT, char* buf) {
	pthread_mutex_lock(&mut);
	while (ready)
		pthread_cond_wait(&con, &mut);
	ready = 1;
	if (!ready1) {
		pass = passV, ready1 = 1, ready0 = passT = (passType = passT) % 2,
			pthread_mutex_unlock(&mut), pthread_cond_signal(&con0);
		if (passT) return 0;
	}
	else {
		passIn = passV;
		if ((passTypeIn = passT) % 2) {
			pthread_mutex_unlock(&mut); 
			return 0;
		}
		ready1 = 0;
		while (!ready1)
			pthread_cond_wait(&con1, &mut);
		ready0 = ready = 0, pthread_mutex_unlock(&mut), pthread_cond_signal(&con0);
	}
	pthread_mutex_lock(&mut);
	while (!ready0)
		pthread_cond_wait(&con0, &mut);
	int out = charCp(buf, readB);
	ready0 = 0, pthread_mutex_unlock(&mut), pthread_cond_signal(&con0);
	return out;
}

static void get() {
	pthread_mutex_lock(&mut);
	if (!ready0) {
		ready0 = 1; pthread_mutex_unlock(&mut);
		pthread_cond_signal(&con0);
		pthread_mutex_lock(&mut);
		while (ready0) pthread_cond_wait(&con0, &mut);
		ready0 = 1;
	}
	if (ready) {
		pass = passIn, passType = passTypeIn;
		if (ready1) ready = 0;
		else {
			ready1 = 1;
			pthread_mutex_unlock(&mut);
			pthread_cond_signal(&con1);
			pthread_mutex_lock(&mut);
			while (ready) pthread_cond_wait(&con0, &mut);
		}
	}
	else {
		ready1 = 0;
		while (!ready1) pthread_cond_wait(&con0, &mut);
		ready = 0;
	}
	pthread_mutex_unlock(&mut);
	pthread_cond_signal(&con);
}

static char prep(rcStrStr** p) {
	char* b;
	int* l;
	if (passType / 2 == rcType_rcStr) *p = (rcStrStr*)pass, b = (*p)->b, l = &(*p)->l;
	else {
		b = (*p = rcStrGet())->b, l = &(*p)->l;
		switch (passType / 2) {
		case rcType_string: *l = charCp(b, (char*)pass); break;
		case rcType_dvb: *l = dvbStrF(b, (int)(unsigned long long)(pass)); break;
		case rcType_stringOn: *l = charCp(b, (char*)pass); *l += charCp(b + *l, " on");  break;
		case rcType_stringOff: *l = charCp(b, (char*)pass); *l += charCp(b + *l, " off");  break;
		case rcType_stringAdd: *l = rcAddcp(b, (char*)pass); break;
		}
	}
	b[(*l)++] = '\n';
	return 1 - passType % 2 * 2;
}

static void waitRepl() {
	pthread_mutex_lock(&mut);
	char flag = 0;
	while (!ready0 && (flag = 1)) pthread_cond_wait(&con0, &mut);
	if (flag) ready0 = 0;
	pthread_mutex_unlock(&mut);
	if (flag) pthread_cond_signal(&con0);
}

static char readStream(int bufL) {
	static char flagR = 0, flagS = 0;
	if (!bufL && !(flagR = flagS = 0)) return 0;
	char* buf = serv.buf, in;
	for (int t = 0; t < bufL && !((in = buf[t]) == ' ' && flagS && !(bufL = flagS = 0)); t++)
		switch (in) {
		case '\n':
			if (flagS) flagS = 0, readAdd('>');
			flagR = 0, readB[readL] = '\0', readL = 0;
			break;
		case '\r':
			if (flagR) readB[readL = 0] = '\0';
			else if ((flagR = 1) && flagS) flagS = 0, readAdd('>');
			break;
		case '>':
			if (flagR) readB[flagR = readL = 0] = '\0';
			else if (flagS) flagS = 0, readAdd('>');
			if (readL) readAdd('>');
			else flagS = 1;
			break;
		default:
			if (flagR) readB[flagR = readL = 0] = '\0';
			else if (flagS) flagS = 0, readAdd('>');
			readAdd(in);
		}
	return bufL;
}