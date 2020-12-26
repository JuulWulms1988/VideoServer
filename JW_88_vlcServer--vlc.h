#include "JW_88_vlcServer.h"

static pthread_mutex_t srcSetMut;

static char init = 0, source[1000] = { '\0' };
static signed char srcSet = 0, srcState;
static int sourceL = 0;
static unsigned int srcSetTime;
static pthread_mutex_t vlcPauzeMut = PTHREAD_MUTEX_INITIALIZER;
static volatile char vlcPauze = 0;

static void srcWrkF(void* o);
static void srcClose();
static vlcModEn srcModF();
static void srcPssTypeF();

static void sleepOne(void* x) { sleep(1); }
static char retF() { char x; if (x = srcSet < 0) srcSet = 0, pthread_mutex_unlock(&srcSetMut); return x; }

vlcModEn vlcMode = vlcMod_off;

char vlcSrcCF(char* buf, int bufL) { return buf[0] == 'v' && buf[1] == 'l' && buf[2] == 'c' && buf[3] == ' ' && buf[4] == '\"' && bufL > 5 && buf[bufL - 1] == '\"'; }
char vlcOffCF(char* buf) { return vergCharP(buf, "vlc OFF"); }
char vlcSysOffCF(char* buf) { return vergCharP(buf, "sys OFF"); }