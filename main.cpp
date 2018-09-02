#include <iostream>

#include "tcp_server.h"

bool g_run = true;

int main() {
	TcpServer server;
	while (g_run) {
		server.run();
		std::cout << "start" << std::endl;
	}
	return 0;
}