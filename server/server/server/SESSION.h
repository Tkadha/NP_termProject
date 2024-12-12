#pragma once
#include "Common.h"
#include "protocol.h"
#include "Player.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib ¸µÅ©

enum E_STATE {E_ONLINE, E_OFFLINE};
class SESSION
{
public:
	SOCKET sock;
	E_STATE state;
	int id;
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;
	int remain_data{};
	CPlayer p{};

public:
	SESSION()
	{
		id = -1;
		state = E_OFFLINE;
		team_color = OBSERVER;
		memset(p.name, 0, sizeof(p.name));
	}
	void ResetSESSION();
	void DoRecv();
	void SendLoginPacket(int pid);
	void SendLogoutPacket(int pid);
	void SendPlayerTeamPacket(int pid, E_TEAMCOLOR color);
	void SendMapPacket(int pid, E_MAPTYPE maptype);
	void SendNamePacket(int pid, char* names);
	void SendStartPacket();
	void SendPosPacket(int pid, double x, double y, E_OBJTYPE objtype);
	void SendScenePacket(int pid, E_SCENEKIND scene);
	void SendEventPacket(E_EVENTTYPE eventtype, char onoff);
	void SendScorePacket(int pid, int red, int blue);
};
