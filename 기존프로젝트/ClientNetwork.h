#pragma once
#include "stdafx.h"
#include "protocol.h"
#define BUFFER_SIZE 1024

class NetWorkManager
{

public:
	
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE] = { 0 };
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;

	NetWorkManager();
	~NetWorkManager();

	void Con();
	void DoRecv();
	bool SendColorPacket(int id, E_TEAMCOLOR color);
	bool SendMapPacket(int id, E_MAPTYPE maptype);
};