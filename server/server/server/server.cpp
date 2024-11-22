#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <iostream>
#include <thread>
#include <array>
#include "SESSION.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
#define SERVERPORT 9000
#define MAXPLAYER  10



// 소켓 함수 오류 출력 후 종료
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

std::array <SESSION, MAXPLAYER> player;
E_MAPTYPE maptype = SOCCER;

void ProcessPacket(int id, char* packet)
{
	switch (packet[1])
	{
	case CS_TEAM_CHOICE: {
		TEAM_PACKET* p = reinterpret_cast<TEAM_PACKET*>(packet);
		player[id].team_color = p->teamcolor;
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (player[i].state == E_OFFLINE) continue;
			player[i].SendPlayerTeamPacket(id, player[id].team_color);
		}
	}
		break;

	case CS_MAP_CHOICE: {
		MAP_PACKET* p = reinterpret_cast<MAP_PACKET*>(packet);
		maptype = p->maptype;
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (player[i].state == E_OFFLINE) continue;
			player[i].SendMapPacket(id, maptype);
		}
		break;
	}
	case CS_NAME: {
		NAME_PACKET* p = reinterpret_cast<NAME_PACKET*>(packet);
		strcpy(player[id].name, p->name);		
		break;
	}
	}
}

void PlayerThread(int id)
{
	player[id].SendLoginPacket(id);
	for (int i = 0; i < MAXPLAYER; ++i) {
		if (player[i].state == E_ONLINE)
			player[id].SendLoginPacket(i);
	}
	player[id].state = E_ONLINE;
	printf("make thread\n");
	while (1) {
		player[id].DoRecv();
		ProcessPacket(id, player[id].recv_buf);
	}
}


int main()
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	std::thread p_thread;
	int id = 0;
	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			closesocket(client_sock);
			closesocket(listen_sock);
			WSACleanup();
			return 0;
		}
		
		player[id].state = E_ONLINE; 
		player[id].sock = client_sock;
		p_thread = std::thread(PlayerThread, id);	
		++id;
		
	}

	closesocket(client_sock);
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}
