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

	void SendColorPacket(E_TEAMCOLOR color);
	void SendMapPacket(E_MAPTYPE maptype);
	void SendNamePacket(const char* name);
	void SendKeyPacket(WPARAM wParam);
	void SendStartPacket();
	void SendExitPacket();
};