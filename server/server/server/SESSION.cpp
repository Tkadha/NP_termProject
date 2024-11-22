#include "SESSION.h"


void SESSION::DoRecv()
{
	int retval;
	memset(recv_buf, 0, sizeof(recv_buf));
	retval = recv(sock, recv_buf, BUFSIZE, 0);
	
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


