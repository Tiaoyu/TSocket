#include "SocketClient.h"
#include <Utils.h>
#include "PBParserClient.h"
void SocketClient::Recv()
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

		PBParserClient::get_instance().ParserPB(sclient, msgInfo.recData + 4, msgInfo.MsgSize);

		// 将剩余字节复制到buffer起始处
		msgInfo.SkipSize = curBuffSize - msgInfo.MsgSize - 4;
		memcpy(msgInfo.recData, msgInfo.recData + msgInfo.MsgSize + 4, msgInfo.SkipSize);
		msgInfo.MsgSize = 0;
	}
}
