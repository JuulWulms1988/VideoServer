#include "JW_88_vlcServer.h"

static FILE* pVlc, *pYdl;
static char buf[1500];
static int bufL;