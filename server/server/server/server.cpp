#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <thread>
#include <algorithm>
#include "GameFramework.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
#define SERVERPORT 9000

int UserInGame();
E_TEAMCOLOR GetLessTeam();

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

CGameFramework game{};
E_MAPTYPE maptype = SOCCER;

HANDLE event_logic;

void ProcessPacket(int id, char* packet)
{
	switch (packet[1])
	{
	case CS_TEAM_CHOICE: {
		TEAM_PACKET* p = reinterpret_cast<TEAM_PACKET*>(packet);
		game.players[id].team_color = p->teamcolor;
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendPlayerTeamPacket(id, game.players[id].team_color);
		}
		break;
	}

	case CS_MAP_CHOICE: {
		MAP_PACKET* p = reinterpret_cast<MAP_PACKET*>(packet);
		maptype = p->maptype;
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendMapPacket(id, maptype);
		}
		break;
	}
	case CS_NAME: {
		NAME_PACKET* p = reinterpret_cast<NAME_PACKET*>(packet);
		strcpy(game.players[id].p.name, p->name);
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendNamePacket(id, game.players[id].p.name);
		}
		break;
	}
	case CS_START: {		// event_ logic 깨우기 작성하기
		//SetEvent(event_logic);
		game.SwitchScene(&game.playScene);

		SCENE_PACKET* p = reinterpret_cast<SCENE_PACKET*>(packet);
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendScenePacket(id, PLAY);
		}
		break;
	}
	case CS_EXIT: {		// event_ logic 깨우기 작성하기
		//SetEvent(event_logic);
		START_PACKET* p = reinterpret_cast<START_PACKET*>(packet);
		game.players[p->id].state = E_OFFLINE;
		if (UserInGame() == -1) {
			game.SwitchScene(&game.lobbyScene);
			break;
		}
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
		}
		break;
	}
	case CS_KEY: {
		KEY_PACKET* p = reinterpret_cast<KEY_PACKET*>(packet);
		if (game.players[id].p.KeyDownBuffer[p->key]) {
			game.players[id].p.KeyDownBuffer[p->key] = false;
		}
		else
			game.players[id].p.KeyDownBuffer[p->key] = true;
		break;
	}
	}
}

void PlayerThread(int id)
{
	game.players[id].id = id;
	game.players[id].team_color = GetLessTeam();
	game.players[id].SendLoginPacket(id);
	game.players[id].SendPlayerTeamPacket(id, game.players[id].team_color);

	printf("player %d : %d\n", id, game.players[id].team_color);

	for (int i = 0; i < MAXPLAYER; ++i) {	
		if (game.players[i].state == E_ONLINE) {
			game.players[id].SendLoginPacket(i);
			game.players[id].SendPlayerTeamPacket(i, game.players[id].team_color);

		}
	}
	printf("%d make thread\n",id);
	while (1) {
		game.players[id].DoRecv();
		if (game.players[id].state == E_OFFLINE) {
			for (int i = 0; i < MAXPLAYER; ++i) {
				if (game.players[i].state == E_ONLINE || id != i) {
					game.players[i].SendLogoutPacket(id);
				}
			}
			break;
		}
		ProcessPacket(id, game.players[id].recv_buf);
	}
	game.players[id].ResetSESSION();
}

void LogicThread()
{
	while (1) {
		game.Update();
		Sleep(1);
	}
}

int UserInGame()
{
	for (int i = 0; i < MAXPLAYER; ++i) 
		if (game.players[i].state == E_OFFLINE) 
			return i;		
	return -1;
}

E_TEAMCOLOR GetLessTeam()
{
	const int redplayers = std::count_if(game.players.begin(), game.players.end(), [](SESSION p) {
		if (p.state == E_OFFLINE)
			return false;
		return p.team_color == RED;
		});

	const int blueplayers = std::count_if(game.players.begin(), game.players.end(), [](SESSION p) {
		if (p.state == E_OFFLINE)
			return false;
		return p.team_color == BLUE;
		});

	printf("red : %d명\tblue : %d명", redplayers, blueplayers);

	if (redplayers > blueplayers)
		return BLUE;
	else
		return RED;
}

int main()
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	event_logic = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (event_logic == NULL) err_quit("CreateEvent()");


	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");


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
	std::thread p_thread, logic_thread;

	logic_thread = std::thread(LogicThread); // 여기 스레드 함수도 쓰고
	logic_thread.detach();

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
		int id = UserInGame();
		if (id != -1) {
			game.players[id].state = E_ONLINE;
			game.players[id].sock = client_sock;
			p_thread = std::thread(PlayerThread, id);
			p_thread.detach();
		}
	}

	closesocket(client_sock);
	closesocket(listen_sock);
	WSACleanup();
	CloseHandle(event_logic);
	return 0;
}