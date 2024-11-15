#pragma once
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include "protocol.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
#define BUFSIZE    512


enum E_STATE {E_ONLINE, E_OFFLINE};
class SESSION
{
public:
	SOCKET sock;
	E_STATE state;
	int id;
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;

public:
	SESSION()
	{
		id = -1;
		state = E_OFFLINE;
		team_color = BLUE;
	}
	void DoRecv();
	bool SendPlayerTeamPacket(int id, E_TEAMCOLOR color);
};


