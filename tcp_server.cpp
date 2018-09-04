#include "tcp_server.h"
#include <algorithm>
#include <iostream>
#include <thread>

namespace Keane {
void close(int fd);

int CellServer::clientCount() {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_clients.size();
}

void  CellServer::addClient(Client *client) {
	if (client == nullptr) {
		return;
	}
	std::lock_guard<std::mutex> lock(m_mutex);
	m_buffClients.push_back(client);
}
void CellServer::onRun() {
	fd_set fds;
	FD_ZERO(&fds);
	while (1) {
		int max_socket = 0;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			for (auto c : m_buffClients) {
				m_clients.emplace_back(c);
				FD_SET(c->m_socket, &fds);
			}
			m_buffClients.clear();

			if (m_clients.empty()) {
				//std::cout << "waiting client to connect";
				continue;
			}
			for_each(m_clients.begin(), m_clients.end(), [&max_socket](auto &c) { if (c->m_socket > max_socket) { max_socket = c->m_socket; } });
		}
		if (select(max_socket + 1, &fds, nullptr, nullptr, nullptr) == INVAILD_SOCKET) {
			//std::cout << WSAGetLastError(); //»ñÈ¡´íÎóÂë
			std::cerr << "run error";
			closeAll();
			return;
		}
		for (int fd = 0; fd < max_socket; ++fd) {
			if (FD_ISSET(fd, &fds)) {
				if (recvData(fd) == INVAILD_SOCKET) {
					std::cerr << "can not recv";
					FD_CLR(fd, &fds);
					closeClient(fd);
				}
			}
		}
	}
}

void CellServer::closeClient(int fd) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = std::find_if(m_clients.begin(), m_clients.end(), [&fd](auto &x)
			 { if (x->m_socket == fd) { return true; } return false; });
	if (it != m_clients.end()) {
		close((*it)->m_socket);
		delete (*it);
		m_clients.erase(it);
	}
}

void CellServer::closeAll() {
	std::lock_guard<std::mutex> lock(m_mutex);
	for (auto x : m_clients) {
		close(x->m_socket);
		delete(x);
	}
	m_clients.clear();
}

short CellServer::recvData(int fd) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = std::find_if(m_buffClients.begin(), m_buffClients.end(), [&fd](auto &c)
	{ if (c->m_socket == fd) { return true; } return false; });
	if (it == m_buffClients.end()) {
		return 0;
	}
	return  0;
}

TcpServer::TcpServer() {
	m_socket = INVAILD_SOCKET;
}

void TcpServer::onRun() {
	if (m_socket == INVAILD_SOCKET) {
		if (initSocket() != OK_SOCKET) {
			return;
		}
	}
	m_cells.resize(CELL_SERVER_NUM);
	for (int i = 0; i < CELL_SERVER_NUM; ++i) {
		m_cells[i] = new CellServer();
	}
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_socket, &fds);
	sockaddr_in client_addr;
	int len = sizeof(client_addr);
	for (int i = 0; i < CELL_SERVER_NUM; ++i) {
		std::thread t(&CellServer::onRun, m_cells[i]);
		t.detach();
	}
	std::cout << "start run";
	while (1) {
		if (select(m_socket + 1, &fds, nullptr, nullptr, nullptr) == INVAILD_SOCKET) {
#ifdef _WIN32
			std::cerr << "select error, thread id" << GetCurrentThreadId();
#else
			std::cerr << "select  error thread id" << pthread_self(); // or gettid()
#endif
			break;
		}
		if (FD_ISSET(m_socket, &fds)) {
				int socket = accept(m_socket, (sockaddr*)&client_addr, &len);
				if (socket == INVAILD_SOCKET) {
					continue;
				}
				std::cout << "recv client from" << inet_ntoa(client_addr.sin_addr) << std::endl;
				Client *client = new Client(socket);
				addClientToServer(client);
		}
	}
}

void TcpServer::addClientToServer(Client *client) {
	CellServer *cell = m_cells[0];
	int count = cell->clientCount();
	for (auto x : m_cells) {
		if (x->clientCount() < count) {
			count = x->clientCount();
			cell = x;
		}
	}
	cell->addClient(client);
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
	if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVAILD_SOCKET) {
		std::cerr << "can not socket";
	}
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
#ifdef _WIN32
	server_addr.sin_addr.S_un.S_addr = inet_addr(IP);
#else
	server_addr.sin_addr.s_addr = inet_addr(IP);
#endif
	if (::bind(m_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == INVAILD_SOCKET) {
		//std::cout << WSAGetLastError();
		close(m_socket);
		std::cerr << "bind error";
		return -1;
	}
	if (::listen(m_socket, LISTEN_NUM) == INVAILD_SOCKET) {
		close(m_socket);
		std::cerr << "listen error";
		return -1;
	}
#ifdef __linux_
	ret = 0;
	if (setsocketopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret)) == INVAILD_SOCKET) {
		std::cerr << "can not set reused, function and  line" << _FUNCTION_ << _LINE_;
	}
#endif
	return OK_SOCKET;
}

void close(int socket) {
	if (socket == INVAILD_SOCKET) {
		return;
	}
	#ifdef _WIN32
		closesocket(socket);
	#else
		close(socket);
	#endif
}
}

