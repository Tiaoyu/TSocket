#pragma once
#include <map>
#include <iostream>
#include <string>

class PBParser
{
private:
	PBParser() {
		std::cout << "PBParser constructor called." << std::endl;
	}
public:
	~PBParser() {
		std::cout << "PBParser destructor called." << std::endl;
	}
	PBParser(const PBParser&) = delete;
	PBParser& operator=(const PBParser&) = delete;
	static PBParser& get_instance() {
		static PBParser instance;
		return instance;
	}

	void ParserPB(int client, char* buffer, int size);
};