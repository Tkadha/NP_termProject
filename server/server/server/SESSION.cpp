#include "SESSION.h"


void SESSION::ResetSESSION()
{
	closesocket(sock);
	state = E_OFFLINE;
	id = -1;
	memset(p.name, 0, sizeof(p.name));
	p.InfoReset();
}

void SESSION::DoRecv()
{
	int retval;
	memset(recv_buf, 0, sizeof(recv_buf));
	retval = recv(sock, recv_buf, BUFSIZE, 0);
	if (retval == 0 || retval == -1) {
		state = E_OFFLINE;
		printf("logout\n");
	}
}

bool SESSION::SendLoginPacket(int pid)
{
	LOGIN_PACKET p;
	p.size = sizeof(LOGIN_PACKET);
	p.type = SC_LOGIN;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}

bool SESSION::SendPlayerTeamPacket(int pid, E_TEAMCOLOR color)
{
	TEAM_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.teamcolor = color;
	p.type = SC_MAP_CHOICE;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}

bool SESSION::SendMapPacket(int pid, E_MAPTYPE maptype)
{
	MAP_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.maptype = maptype;
	p.type = SC_MAP_CHOICE;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}

bool SESSION::SendNamePacket(int pid, char* names)
{
	NAME_PACKET p;
	p.size = sizeof(NAME_PACKET);
	strcpy(p.name, names);
	p.type = SC_NAME;
	p.id = pid;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}

bool SESSION::SendStartPacket()
{
	START_PACKET p;
	p.size = sizeof(START_PACKET);
	p.type = SC_START;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}

bool SESSION::SendPosPacket(int pid, double x, double y, E_OBJTYPE objtype)
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
	if (retval == SOCKET_ERROR) return false;

	printf("%f,%f\n", p.x, p.y);
	return true;
}


