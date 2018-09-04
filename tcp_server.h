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
#include <mutex>

#define CELL_SERVER_NUM  4
#define INVAILD_SOCKET  ~0
#define OK_SOCKET 0
#define PORT 6677
#define LISTEN_NUM 1024

namespace Keane {
	class Client {
		friend class CellServer;
	public:
		Client(int fd);
	private:
		int m_socket;
	};
	class CellServer {
	public:
		void addClient(Client *client);
		void onRun();
		void clientCount();

	private:
		void closeAll();
		short recvData(int fd);
		void closeClient(int fd);

	private:
		std::mutex m_mutex;
		std::vector<Client *> m_buffClients;
		std::vector<Client *> m_clients;
	};
	class TcpServer {
	public:
		TcpServer();
		void virtual onRun();

	private:
		short initSocket();
		void addClientToServer(Client *client);
		void close();
	private:
		int m_socket;
		std::vector<CellServer *> m_cells;
	};
}
#endif