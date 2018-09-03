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
	#include <unistd.h>
#endif
#include <vector>
#define CELL_SERVER_NUM  4
#define INVAILD_SOCKET  ~0
#define ERROR_SOCKET -1
#define OK_SOCKET 0
#define PORT 6677
#define LISTEN_NUM 1024
class Client {

};
class CellServer {

};
class TcpServer {
public:
	TcpServer();
	void virtual onRun();


private:
	short initSocket();
private:
	int m_socket;
	std::vector<CellServer *> m_cells;
};

#endif