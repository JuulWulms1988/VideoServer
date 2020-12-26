#include "JW_88_vlcServer--pip.h"

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void pipOpen(char* x, int y, char* cmd) {
	pthread_mutex_lock(&mut);
	FILE** z; char* p; switch (y) {
	case 0: p = "w", z = &pVlc; break;
	case 1: p = "r", z = &pYdl; break;
	} while ((*z = popen(x, p)) == NULL);
	charCp(x, "pidof -x ");
	{
	int l = 9 + charCp(x + 9, cmd);
	charCp(x + l, " > /dev/null");
	}
	while (system(x)) usleep(10000);
	pthread_mutex_unlock(&mut);
}

void pipForwardVlc(short x) {
	pthread_mutex_lock(&mut);
	FILE * to;
	switch (x) {
	case 0:
		to = pYdl;
		break;
	}
	while ((bufL = fread(buf, 1, 1, pYdl)) > 0)
		fwrite(buf, 1, bufL, pVlc), fflush(pVlc);
	pclose(pYdl);
	pthread_mutex_unlock(&mut);
}