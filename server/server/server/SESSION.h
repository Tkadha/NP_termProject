#pragma once
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include "protocol.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ


enum E_STATE {E_ONLINE, E_OFFLINE};
class SESSION
{
public:
	SOCKET sock;
	E_STATE state;
	int id;
	char recv_buf[BUFSIZE];
	E_TEAMCOLOR team_color;
	char name[NAME_SIZE];

	char name[20]{};
	XY position, velocity;
	BOOL KetDownBuffer[256];
	double friction;		// ���� ���
	int size;
	double power = 16;
	bool input{ false }, hasKicked{ false };

public:
	SESSION()
	{
		id = -1;
		state = E_OFFLINE;
		team_color = BLUE;
		memset(name, 0, sizeof(name));
	}
	void DoRecv();
	bool SendLoginPacket(int pid);
	bool SendPlayerTeamPacket(int pid, E_TEAMCOLOR color);
	bool SendMapPacket(int pid, E_MAPTYPE maptype);
	bool SendNamePacket(int pid, char* names);
};


