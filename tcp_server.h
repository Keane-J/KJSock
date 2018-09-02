#ifndef __JKSOCKET_TCP_SERVER_H
#define __JKSOCKET_TCP_SERVER_H

#ifdef _WIN32
	#include <WinSock2.h>
	#include <Windows.h>
	#pragma  comment(lib, "ws2_32.lib")
#else
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

class TcpServer {
public:
	void virtual run() { };
};

#endif