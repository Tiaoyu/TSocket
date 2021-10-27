#include "SocketBase.h"
#include <Utils.h>
using namespace std;

void SocketBase::Connect(string ip, int port)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0) {
		return;
	}

	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET) {
		printf("invalid socket!");
		return;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	inet_pton(AF_INET, ip.c_str(), &serAddr.sin_addr);
	if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		printf("Connect error!");
		closesocket(sclient);
		return;
	}
}

void SocketBase::Recv()
{
	while (true) {
		int ret = recv(sclient, msgInfo.recData + msgInfo.SkipSize, MAX_MSG_BUFF_SIZE - msgInfo.SkipSize, 0);
		if (ret < 0) {
			continue;
		}
		int curBuffSize = ret + msgInfo.SkipSize;

		msgInfo.recData[ret + msgInfo.SkipSize] = 0x00;

		// 解析协议头
		if (msgInfo.MsgSize == 0)
		{
			// 协议头需要4字节 没到4字节则继续等待数据
			if (curBuffSize < 4) {
				msgInfo.SkipSize += ret;
				continue;
			}
			// 协议长度超出预期 需要直接断连
			Utils::BytesToInt(msgInfo.MsgSize, (byte*)msgInfo.recData, 4, true);
			if (msgInfo.MsgSize > MAX_MSG_SIZE) {
				ClearMsgInfo();
				continue;
			}
		}

		// 协议体需要MsgSize字节 数据不足则继续等待数据
		if (curBuffSize < msgInfo.MsgSize + 4) {
			msgInfo.SkipSize += ret;
			continue;
		}

		//PBParser::get_instance().ParserPB(sclient, msgInfo.recData + 4, msgInfo.MsgSize);

		// 将剩余字节复制到buffer起始处
		msgInfo.SkipSize = curBuffSize - msgInfo.MsgSize - 4;
		memcpy(msgInfo.recData, msgInfo.recData + msgInfo.MsgSize + 4, msgInfo.SkipSize);
		msgInfo.MsgSize = 0;
	}
}


// 协议发送
void SocketBase::Send(void* data, int size, int protoId)
{
	byte head[4] = { 0 };
	Utils::IntToBytes(4 + size, head, 4, true);
	byte sendData[1024] = { 0 };
	memcpy(sendData, head, 4);
	byte pbId[4] = { 0 };
	Utils::IntToBytes(protoId, pbId, 4, true);
	memcpy(sendData + 4, pbId, 4);
	memcpy(sendData + 4 + 4, data, size);
	send(sclient, (const char*)sendData, size + 4 + 4, 0);
}

// 清空字节缓存
void SocketBase::ClearMsgInfo()
{
	msgInfo.MsgSize = 0;
	msgInfo.SkipSize = 0;
}

void SocketBase::SetClient(SOCKET sclient)
{
	this->sclient = sclient;
}
