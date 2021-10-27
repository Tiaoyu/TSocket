#pragma once
#include <iostream>
#include <string>
class PBParserClient
{
private:
	PBParserClient() {
		std::cout << "PBParserClient constructor called." << std::endl;
	}
public:
	~PBParserClient() {
		std::cout << "PBParserClient destructor called." << std::endl;
	}
	PBParserClient(const PBParserClient&) = delete;
	PBParserClient& operator=(const PBParserClient&) = delete;
	static PBParserClient& get_instance() {
		static PBParserClient instance;
		return instance;
	}

	void ParserPB(int client, char* buffer, int size);
};

