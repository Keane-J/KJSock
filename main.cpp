#include <iostream>
#include <thread>
#include "tcp_server.h"

bool g_run = true;

int main() {
	using namespace Keane;
	TcpServer server;
	std::thread t(&TcpServer::onRun, &server);
	t.join();
	Sleep(2000);
	return 0;
}