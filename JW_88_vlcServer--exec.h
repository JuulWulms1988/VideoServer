#include "JW_88_vlcServer.h"

static char execBuf[3000];

static void* open();
static void beginF();
static void func(char* buf, int bufL, int funcTime);

__dynHeapP_ptr execDynHeap;

void execBegin() { static pthread_t p; pthread_create(&p, NULL, open, NULL); pthread_detach(p); beginF(); }
char* execBufgetP(char x) { return execBuf + x * 1000; }
char execBufgetN(char* x) { return (x - execBuf) / 1000; }