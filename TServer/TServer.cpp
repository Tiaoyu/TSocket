// TSocket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "game.pb.h"
#include "SocketServer.h"
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <Utils.h>


void t1(SOCKET sClient) {
	char revData[255];
	SocketServer sock = SocketServer();
	sock.SetClient(sClient);
	sock.Recv();

	return;
}

int main()
{
	std::cout << "Hello World!\n";

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		return 0;
	}

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error!");
		return 0;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("bind error!");
	}

	if (listen(slisten, 5) == SOCKET_ERROR) {
		printf("listen error !");
		return 0;
	}

	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	while (true) {
		printf("Wait to be connected...\n");
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET) {
			printf("accept error !");
			continue;
		}
		char ip[100];
		if (inet_ntop(AF_INET, &remoteAddr.sin_addr.S_un.S_addr, ip, 100) == NULL) {
			printf("ntop error!");
			continue;
		}
		printf("Accepted: %s\n", ip);

		std::thread t(t1, sClient);
		t.join();
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}

