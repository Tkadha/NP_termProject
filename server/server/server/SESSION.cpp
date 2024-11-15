#include "SESSION.h"


void SESSION::DoRecv()
{
	int retval;
	memset(recv_buf, 0, sizeof(recv_buf));
	retval = recv(sock, recv_buf, BUFSIZE, 0);
	
}

bool SESSION::SendPlayerTeamPacket(int id, E_TEAMCOLOR color)
{
	TEAM_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.teamcolor = color;
	p.type = CS_TEAM_CHOICE;
	p.id = id;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
	if (retval == SOCKET_ERROR) return false;
	return true;
}


