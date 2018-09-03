#include "tcp_server.h"
#include <iostream>
TcpServer::TcpServer() {
	m_socket = INVAILD_SOCKET;
}
void TcpServer::onRun() {
	if (m_socket == INVAILD_SOCKET) {
		if (initSocket() != OK_SOCKET) {
			return;
		}
	}
	for (int i = 0; i < CELL_SERVER_NUM; ++i) {
		m_cells[i] = new CellServer();
	}
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_socket, &fds);
	while (1) {
		if (select(m_socket + 1, &fds, nullptr, nullptr, nullptr) == ERROR_SOCKET) {
			break;
		}
		sockaddr_in client_addr;
		int len = sizeof(client_addr);
		int socket = accept(m_socket, (sockaddr*)&client_addr, &len);
		//to do
	}
}

short TcpServer::initSocket() {
#ifdef _WIN32
	WORD wd;
	WSADATA ws_data;
	wd = MAKEWORD(1, 1);
	int ret = WSAStartup(wd, &ws_data);
	if (ret != OK_SOCKET) {
		std::cerr << "can not wsa start up";
	}
#endif
	if (m_socket = socket(AF_INET, SOCK_STREAM, 0)) {
		std::cerr << "can not socket";
	}
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
#ifdef _WIN32
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif
	if (bind(m_socket, (sockaddr *)&server_addr, sizeof(server_addr)) != OK_SOCKET) {
		std::cerr << "bind error";
		return -1;
	}
	listen(m_socket, LISTEN_NUM);
	//ret = 0;
	//if (setsocketopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret))
	return OK_SOCKET;
}
}

