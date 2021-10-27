#include "PBParser.h"
#include "SocketServer.h"
#include <Utils.h>
#include <game.pb.h>
#include <enum.pb.h>

void PBParser::ParserPB(int client, char* buffer, int size)
{
	unsigned char prefixPB[4];
	for (int i = 0; i < 4; ++i) {
		prefixPB[i] = buffer[i];
	}
	int pbId = 0;
	Utils::BytesToInt(pbId, prefixPB, 4, true);

	switch (pbId) {
	case (int)gamepb::EC2SPing:
	{
		gamepb::C2SPing ping = gamepb::C2SPing();
		ping.ParseFromArray(buffer + 4, size - 4);
		printf("Recv from client. PBLen:%d PBId:%d Msg:Ping{%lld}\n", size, pbId, ping.timestamp());
		

		gamepb::S2CPing c2s;
		c2s.set_timestamp(time(0)-ping.timestamp());
		size_t s = c2s.ByteSizeLong();
		void* data = malloc(s);
		c2s.SerializeToArray(data, s);
		SocketServer base = SocketServer();
		base.SetClient(client);
		base.Send(data, size, gamepb::ES2CPING);


		//gamepb::C2SPing c2s;
		//c2s.set_timestamp(time(0));
		//size_t size = c2s.ByteSizeLong();
		//void* data = malloc(size);
		//c2s.SerializeToArray(data, size);
		//SocketServer base = SocketServer();
		//base.SetClient(client);
		//base.Send(data, size, gamepb::ES2CPING);

		break;
	}
	case (int)gamepb::EC2SGobangStep:
	{
		gamepb::C2SGobangStep step = gamepb::C2SGobangStep();
		step.ParseFromArray(buffer + 4, size - 4);
		printf("Recv from client. PBLen:%d PBId:%d Msg:GobangStep{camp(%d),Point(%d,%d)}\n", size, pbId, step.point().camp(), step.point().x(), step.point().y());
		
		gamepb::S2CGobangStep c2s;
		gamepb::Point* p = new (gamepb::Point);
		gamepb::GobangInfo* g = new(gamepb::GobangInfo);
		p->set_camp(step.point().camp());
		p->set_x(step.point().x());
		p->set_y(step.point().y());

		gamepb::ChessStep* s = g->add_chesssteps();
		s->set_index(0);
		s->set_allocated_point(p);
		
		c2s.set_allocated_gobanginfo(g);
		size_t size = c2s.ByteSizeLong();
		void* data = malloc(size);
		c2s.SerializeToArray(data, size);
		SocketServer base = SocketServer();
		base.SetClient(client);
		base.Send(data, size, gamepb::ES2CGobangStep);
		break;
	}
	default:
		break;
	}
}
