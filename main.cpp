#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>
#include "tcp_server.h"
#include "kjlog.h"
#include "rwlock.hpp"

bool g_run = true;

using namespace KeaneJ;
RWMutex mt;
void play() {
	RWLock lock(mt, RWType::RW_READ);
	std::cout << "a";
	//<< GetCurrentThreadId();
	Sleep(200);
	
}
void display() {
	RWLock lock(mt, RWType::RW_WRITE);
	std::cout << "b";
	Sleep(200);
}
#define THREAD_NUM 8
int main() {
	std::thread t[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; ++i) {
		if (i & 0x01) {
			t[i] = std::thread(&play);
		} else {
			t[i] = std::thread(&display);
		}
	}
	for (int i = 0; i < THREAD_NUM; ++i) {
		t[i].detach();
	}
	using namespace Keane;
	KJLOG_INFO << "START RUN";
	TcpServer server;
	std::thread t(&TcpServer::onRun, &server);
	t.join();
	Sleep(200000);
	return 0;
}