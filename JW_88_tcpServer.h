#ifndef JW_88_tcpServer_H_   
#define JW_88_tcpServer_H_

#define TCP_PACK_SIZE 1024

typedef struct TCPServ { 
	const char* ip; 
	const int port; 
	char buf[TCP_PACK_SIZE + 1]; 
}TCPServ;

extern void TCPFunc(TCPServ* serv, char(*pFunc)(int, int), char* oStr, int oStrL);

extern int TCPSend(int sock, char* b, unsigned int l, int f);


#endif