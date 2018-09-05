#include <iostream>
#include <thread>
#include "tcp_server.h"
#include "kjlog.h"

bool g_run = true;

int main() {
	using namespace Keane;
	KLLOG_INFO << "START RUN";
	//TcpServer server;
	//std::thread t(&TcpServer::onRun, &server);
	//t.join();
	Sleep(20000);
	return 0;
}