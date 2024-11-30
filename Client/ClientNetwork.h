#pragma once
#include "../server/server/server/protocol.h"
#include "Common.h"
#include <thread>
#include <array>
#include <string.h>

class NetWorkManager
{

public:

	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in serv_addr;
	int id = -1;
	char recv_buf[BUFSIZE]{};
	E_TEAMCOLOR team_color;
	int remain_data{};

	NetWorkManager();
	~NetWorkManager();

	void Con();

	void DoRecv();

	bool SendColorPacket(E_TEAMCOLOR color);
	bool SendMapPacket(E_MAPTYPE maptype);
	bool SendNamePacket(const char* name);
	bool SendKeyPacket(WPARAM wParam);
	bool SendStartPacket();
	bool SendExitPacket();
};