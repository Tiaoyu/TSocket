#pragma once
#include <string>
class Utils
{
private:
	static int idx;
public:
	static void IntToBytes(int i, unsigned char* bytes, int size = 4, bool isBigEnd = true)
	{
		if (size == 4) {
			memset(bytes, 0, size);
			if (isBigEnd) {
				bytes[3] = (i & 0xff);
				bytes[2] = (i & 0xff00) >> 8;
				bytes[1] = (i & 0xff0000) >> 16;
				bytes[0] = (i & 0xff000000) >> 24;
			}
			else {
				bytes[0] = (i & 0xff);
				bytes[1] = (i & 0xff00) >> 8;
				bytes[2] = (i & 0xff0000) >> 16;
				bytes[3] = (i & 0xff000000) >> 24;
			}
		}
	}

	static void BytesToInt(int& i, unsigned char* bytes, int size, bool isBigEnd)
	{
		int tmp = 0;
		if (isBigEnd) {
			tmp |= bytes[3];
			tmp |= bytes[2] << 8;
			tmp |= bytes[1] << 16;
			tmp |= bytes[0] << 24;
		}
		else {
			tmp |= bytes[0];
			tmp |= bytes[1] << 8;
			tmp |= bytes[2] << 16;
			tmp |= bytes[3] << 24;
		}
		i = tmp;
	}

	static int GetId() { return ++idx; }
};

