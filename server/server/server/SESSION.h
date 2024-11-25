#pragma once
#include "Common.h"
#include "protocol.h"
#include "Player.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

enum E_STATE {E_ONLINE, E_OFFLINE};
class SESSION
{
public:
	SOCKET sock;
	E_STATE state;
	int id;
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;

	CPlayer p{};

public:
	SESSION()
	{
		id = -1;
		state = E_OFFLINE;
		team_color = BLUE;
		memset(p.name, 0, sizeof(p.name));
	}
	void DoRecv();
	bool SendLoginPacket(int pid);
	bool SendPlayerTeamPacket(int pid, E_TEAMCOLOR color);
	bool SendMapPacket(int pid, E_MAPTYPE maptype);
	bool SendNamePacket(int pid, char* names);
	bool SendStartPacket();
};
