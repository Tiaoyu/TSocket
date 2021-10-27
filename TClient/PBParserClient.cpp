#include "PBParserClient.h"
#include <Utils.h>
#include <Utils.h>
#include <game.pb.h>
#include <enum.pb.h>

void PBParserClient::ParserPB(int client, char* buffer, int size)
{
	unsigned char prefixPB[4];
	for (int i = 0; i < 4; ++i) {
		prefixPB[i] = buffer[i];
	}
	int pbId = 0;
	Utils::BytesToInt(pbId, prefixPB, 4, true);

	switch (pbId) {
	case (int)gamepb::ES2CPING:
	{
		gamepb::S2CPing ping = gamepb::S2CPing();
		ping.ParseFromArray(buffer + 4, size - 4);
		printf("Recv from client. PBLen:%d PBId:%d Msg:Ping{%lld}\n", size, pbId, ping.timestamp());
		break;
	}
	case (int)gamepb::ES2CGobangStep:
	{
		gamepb::S2CGobangStep step = gamepb::S2CGobangStep();
		step.ParseFromArray(buffer + 4, size - 4);
		printf("Recv from client. PBLen:%d PBId:%d Msg:GobangStep{camp(%d),Point(%d,%d)}\n", 
			size, 
			pbId, 
			step.gobanginfo().chesssteps()[0].point().camp(), 
			step.gobanginfo().chesssteps()[0].point().x(), 
			step.gobanginfo().chesssteps()[0].point().y());
		break;
	}
	default:
		break;
	}
}
