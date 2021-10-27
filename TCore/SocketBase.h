#pragma once
#include <string>
#include <queue>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <game.pb.h>
#include <enum.pb.h>

class SocketBase
{
protected:
	const int MAX_MSG_SIZE = 1024;       // 协议最大长度
	const int MAX_MSG_BUFF_SIZE = 10240; // 缓存最大长度
	WSADATA wsaData;
	SOCKET sclient;
	SOCKET sockets[1024];
	struct MsgInfo {
		char recData[1024] = { 0 }; // 字节流缓存数组
		int SkipSize = 0;           // 当前已接收但未处理的字节长度
		int MsgSize = 0;            // 当前协议长度
	} msgInfo;
public:
	void ClearMsgInfo();
	void SetClient(SOCKET sclient);
	void Connect(std::string ip, int port);
	void Recv();
	void Send(void* data, int size, int protoId);
	void BytesToInt(int& i, char* bytes, int size, bool isBigEnd);
};

