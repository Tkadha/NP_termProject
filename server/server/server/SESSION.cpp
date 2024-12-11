#include "SESSION.h"


void SESSION::ResetSESSION()
{
	closesocket(sock);
	state = E_OFFLINE;
	team_color = OBSERVER;
	id = -1;
	memset(p.name, 0, sizeof(p.name));
	p.InfoReset();
}

void SESSION::DoRecv()
{
	int retval;
	retval = recv(sock, recv_buf + remain_data, BUFSIZE - remain_data, 0);
	if (retval == 0 || retval == -1) {
		state = E_OFFLINE;
		printf("logout\n");
		return;
	}
	remain_data += retval;
}

void SESSION::SendLoginPacket(int pid)
{
	LOGIN_PACKET p;
	p.size = sizeof(LOGIN_PACKET);
	p.type = SC_LOGIN;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendLogoutPacket(int pid)
{
	LOGIN_PACKET p;
	p.size = sizeof(LOGIN_PACKET);
	p.type = SC_LOGOUT;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendPlayerTeamPacket(int pid, E_TEAMCOLOR color)
{
	TEAM_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.teamcolor = color;
	p.type = SC_TEAM_CHOICE;
	p.id = pid;
	int retval;

	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendMapPacket(int pid, E_MAPTYPE maptype)
{
	MAP_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.maptype = maptype;
	p.type = SC_MAP_CHOICE;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendNamePacket(int pid, char* names)
{
	NAME_PACKET p;
	p.size = sizeof(NAME_PACKET);
	strcpy(p.name, names);
	p.type = SC_NAME;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendStartPacket()
{
	START_PACKET p;
	p.size = sizeof(START_PACKET);
	p.type = SC_START;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendPosPacket(int pid, double x, double y, E_OBJTYPE objtype)
{
	POS_PACKET p;
	p.size = sizeof(POS_PACKET);
	p.type = SC_POS;
	p.id = pid;
	p.x = x;
	p.y = y;
	p.objtype = objtype;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}


void SESSION::SendScenePacket(int pid, E_SCENEKIND scene)
{
	SCENE_PACKET p;
	p.size = sizeof(SCENE_PACKET);
	p.type = SC_SCENE;
	p.scenekind = scene;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void SESSION::SendEventPacket(E_EVENTTYPE eventtype, char onoff)
{
	EVENT_PACKET p;
	p.size = sizeof(EVENT_PACKET);
	p.type = SC_EVENT;
	p.eventtype = eventtype;
	p.onoff = onoff;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}



