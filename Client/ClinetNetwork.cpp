#include "ClientNetwork.h"
#pragma comment(lib, "ws2_32")
#include <iostream>
#define PORT 9000
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


NetWorkManager::NetWorkManager()
{

}
NetWorkManager::~NetWorkManager()
{
}
void NetWorkManager::Con() {

	//const char* server_ip = "58.228.11.233";
	const char* server_ip = "127.0.0.1";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

		err_quit("WSAStartup()");

	}

	sock = socket(AF_INET, SOCK_STREAM, 0);


	if (sock == INVALID_SOCKET) {

		err_quit("socket()");
			
	}

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {

		err_display("inet_pton()");

		closesocket(sock);

		WSACleanup();

	}
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {

		err_display("connect()");

		closesocket(sock);

		WSACleanup();

	}
}


void NetWorkManager::DoRecv()
{
	int retval;
	retval = recv(sock, recv_buf + remain_data, BUFSIZE - remain_data, 0);
	remain_data += retval;
}

void NetWorkManager::SendColorPacket(E_TEAMCOLOR color) {
	TEAM_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.teamcolor = color;
	p.type = CS_TEAM_CHOICE;
	p.id = id;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}
void NetWorkManager::SendMapPacket(E_MAPTYPE maptype)
{
	MAP_PACKET p;
	p.size = sizeof(TEAM_PACKET);
	p.maptype = maptype;
	p.type = CS_MAP_CHOICE;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void NetWorkManager::SendNamePacket(const char* name)
{
	NAME_PACKET p;
	p.size = sizeof(NAME_PACKET);
	strcpy(p.name, name);
	p.type = CS_NAME;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void NetWorkManager::SendKeyPacket(WPARAM wParam)
{
	KEY_PACKET p;
	p.size = sizeof(KEY_PACKET);
	p.type = CS_KEY;
	p.id = id;

	p.key = wParam;
	
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void NetWorkManager::SendStartPacket()
{
	START_PACKET p;
	p.size = sizeof(START_PACKET);
	p.type = CS_START;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}

void NetWorkManager::SendExitPacket()
{
	START_PACKET p;
	p.size = sizeof(START_PACKET);
	p.type = CS_EXIT;
	int retval;
	retval = send(sock, reinterpret_cast<char*>(&p), p.size, 0);
}
