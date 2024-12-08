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
	bool SendLoginPacket(int pid);
	bool SendLogoutPacket(int pid);
	bool SendPlayerTeamPacket(int pid, E_TEAMCOLOR color);
	bool SendMapPacket(int pid, E_MAPTYPE maptype);
	bool SendNamePacket(int pid, char* names);
	bool SendStartPacket();
	bool SendPosPacket(int pid, double x, double y, E_OBJTYPE objtype);
	bool SendScenePacket(int pid, E_SCENEKIND scene);
	bool SendEventPacket(E_EVENTTYPE eventtype, char onoff, char way);
	bool SendEventPacket(E_EVENTTYPE eventtype, char onoff, CRectangleObject& obs);
};
