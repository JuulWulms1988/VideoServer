#include "JW_88_tcpServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int TCPSend(int sock, char* b, unsigned int l, int f) { return send(sock, b, l, f); }

void TCPFunc(TCPServ* serv, char(*pFunc)(int, int), char* oStr, int oStrL) {
	char isOpen = 0;
	int sock;
	for (;;) {
		if (isOpen) close(sock), sleep(1); else isOpen = 1;
		struct sockaddr_in server;
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			isOpen = 0, sleep(3);
			continue;
		}
		server.sin_addr.s_addr = inet_addr(serv->ip);
		server.sin_family = AF_INET;
		server.sin_port = htons(serv->port);
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) continue;
		if (oStrL && send(sock, oStr, oStrL, 0) < 0) continue;
		char rst = 1;
		int bufL;
			do bufL = recv(sock, serv->buf, TCP_PACK_SIZE - 1, 0);
		while (bufL > 0	&& pFunc(bufL, sock * (1 - rst * 2) - (rst && !(rst = 0))));
	}
}