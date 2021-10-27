// TClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>
#include "SocketClient.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void t1(SocketClient sock) {
	sock.Recv();
	return;
}

void t2(SocketClient sock) {
	while (true) {
		int pId = 0;
		std::cin >> pId;
		switch (pId) {
		case 0:
		{
			gamepb::C2SPing c2s;
			c2s.set_timestamp(time(0));
			size_t size = c2s.ByteSizeLong();
			void* data = malloc(size);
			c2s.SerializeToArray(data, size);
			sock.Send(data, size, gamepb::EC2SPing);
			printf("Ping to server. PingTime:%lld\n", c2s.timestamp());
			break;
		}
		case 2:
		{
			gamepb::C2SGobangStep c2s;
			gamepb::Point* p = new(gamepb::Point);
			p->set_camp(1);
			p->set_x(2);
			p->set_y(3);
			c2s.set_allocated_point(p);

			size_t size = c2s.ByteSizeLong();
			void* data = malloc(size);
			c2s.SerializeToArray(data, size);
			sock.Send(data, size, gamepb::EC2SGobangStep);
			printf("GobangStep to server. GobangStep:%s\n", c2s.SerializeAsString().c_str());
			break;
		}
		default:break;
		}
	}
}
int main()
{
	//return Connect();
	SocketClient sock = SocketClient();
	sock.Connect("127.0.0.1", 8888);
	std::thread tt1(t1, sock);
	std::thread tt2(t2, sock);
	tt1.join();
	tt2.join();
}