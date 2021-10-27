#pragma once
#include <map>
#include <iostream>
class UserMgr
{
private :
	UserMgr() {
		std::cout << "UserMgr constructor called." << std::endl;
	}

	std::map<int, int> userMap;
public :
	~UserMgr() {
		std::cout << "UserMgr destructor called." << std::endl;
	}

	UserMgr(const UserMgr&) = delete;
	UserMgr& operator=(const UserMgr&) = delete;
	static UserMgr& get_instance() {
		static UserMgr instance;
		return instance;
	}

	void InsertUserMap(int client, int id);
};

