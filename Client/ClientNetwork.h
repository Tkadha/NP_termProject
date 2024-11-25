#pragma once
#include "../server/server/server/protocol.h"
#include "Common.h"
#include <thread>
#include <array>
#define BUFFER_SIZE 1024

class NetWorkManager
{

public:

	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE] = { 0 };
	int id = -1;
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;


	NetWorkManager();
	~NetWorkManager();

	void Con();

	void DoRecv();

	bool SendColorPacket(E_TEAMCOLOR color);
	bool SendMapPacket(E_MAPTYPE maptype);
	bool SendNamePacket(char* name);
	bool SendKeyPacket(WPARAM wParam);
};