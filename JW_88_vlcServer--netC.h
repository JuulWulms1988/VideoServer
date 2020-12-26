#include "JW_88_vlcServer.h"

static TCPServ serv = { "192.168.180.31", 10003 };

static char* bufPss;
static int bufPssL;

static void netCFunc(int sock);
static char checkBsF(int bufL, int sock);
static char checkVlcF();