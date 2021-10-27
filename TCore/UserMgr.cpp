#include "UserMgr.h"

void UserMgr::InsertUserMap(int client, int id)
{
	userMap[client] = id;
}
