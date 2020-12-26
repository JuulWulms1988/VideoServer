#include "JW_88_vlcServer.h"

rcStrStr rcStrS[4] = { { 0, { '\0' } }, { 0, { '\0' } }, { 0, { '\0' } }, { 0, { '\0' } } };

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER, strMut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t con = PTHREAD_COND_INITIALIZER, con0 = PTHREAD_COND_INITIALIZER, con1 = PTHREAD_COND_INITIALIZER;
static TCPServ serv = { "127.0.0.1", 10000 };
static char ready = 0, ready0 = 0, ready1 = 1, readB[128], strCnt = 4, strAr[4] = { 0, 1, 2, 3 };
static int readL = 0; 
static rcTypeEn passType = 0, passTypeIn;
static void* pass, *passIn;

static void* open();
static void get();
static char netFunc(int bufL, int sock);
static void get();
static char prep(rcStrStr** p);
static void waitRepl();
static char readStream(int bufL);

static void readAdd(char x) { if (readL > 100) readL = 0; readB[readL++] = x; }

int rcAddcp(char* x, char* y) { return charCp(x, "add ") + charCp(x + 4, y); }
rcStrStr* rcStrGet() { pthread_mutex_lock(&strMut); rcStrStr* x = rcStrS + strAr[--strCnt]; pthread_mutex_unlock(&strMut); return x; }
void rcStrRet(rcStrStr* x) { pthread_mutex_lock(&strMut); strAr[strCnt++] = x - rcStrS, pthread_mutex_unlock(&strMut); }