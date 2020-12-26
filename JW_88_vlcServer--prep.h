#include "JW_88_vlcServer.h"

typedef struct { basUniF p;	void* o; }workO;
typedef enum { memNet, memWork } member;

static volatile char ready[2] = { 0, 2 };
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t con = PTHREAD_COND_INITIALIZER, conW = PTHREAD_COND_INITIALIZER;
static int netBufL, netBufT;
static char netBuf = 1;

static void readyF();
static void conF(member x, workO* y, char flagForceW2FrcTke3Bth);
static void sysCloseF();
static void pssF(void* o);

static void workF(void* o) { ((workO*)o)->p(((workO*)o)->o); conF(memWork, NULL, 1); }

__workerTh_ prepWorkObj;

char prepCloseV = 0;

void prepBegin() { prepWorkObj = __workerTh_begin(1, 1); }
char prepPassF(char x, int l, unsigned int t, char flagForceW2FrcTke3Bth) { return conF(memNet, &((workO) { pssF, &((struct { char* a; int l; unsigned int t; }) { &x, l, t }) }), flagForceW2FrcTke3Bth), x; }