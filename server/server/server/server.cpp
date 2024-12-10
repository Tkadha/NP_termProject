#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <thread>
#include <algorithm>
#include "GameFramework.h"
#include <random>
#include <chrono>
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크
#define SERVERPORT 9000

int FindRemainID();
E_TEAMCOLOR GetLessTeam();
bool IsGameEnd();
void WindWay(int);

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

HANDLE event_logic;
HANDLE event_event;

void ProcessPacket(int id, char* packet)
{
	switch (packet[1])
	{
	case CS_TEAM_CHOICE: {
		TEAM_PACKET* p = reinterpret_cast<TEAM_PACKET*>(packet);
		game.players[id].team_color = p->teamcolor;
		printf("%d", game.players[id].team_color);
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendPlayerTeamPacket(id, game.players[id].team_color);
		}
		break;
	}

	case CS_MAP_CHOICE: {
		MAP_PACKET* p = reinterpret_cast<MAP_PACKET*>(packet);
		game.ChangeMap(p->maptype);
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendMapPacket(id, p->maptype);
		}
		break;
	}
	case CS_NAME: {
		NAME_PACKET* p = reinterpret_cast<NAME_PACKET*>(packet);
		strcpy(game.players[id].p.name, p->name);
		std::cout << "이름" << p->name << std::endl;
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendNamePacket(id, game.players[id].p.name);
		}
		break;
	}
	case CS_START: {		// event_ logic 깨우기 작성하기
		SetEvent(event_logic);
		game.SwitchScene(&game.playScene);

		SCENE_PACKET* p = reinterpret_cast<SCENE_PACKET*>(packet);
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendScenePacket(id, PLAY);
		}

		break;
	}
	case CS_EXIT: {		// 플레이어가 모두 접속 종료할 경우 로비로 넘어가게 만들기
		/*START_PACKET* p = reinterpret_cast<START_PACKET*>(packet);
		game.players[p->id].state = E_OFFLINE;
		if (UserInGame() == -1) {
			game.SwitchScene(&game.lobbyScene);
			break;
		}
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
		}*/
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
	//if (game.isPlaying())
	//game.players[id].team_color = GetLessTeam();
	//else

	game.players[id].team_color = OBSERVER;

	game.players[id].SendLoginPacket(id);
	if(game.IsPlayScene())
		game.players[id].SendScenePacket(id, PLAY);
	else
		game.players[id].SendScenePacket(id, LOBBY);
	game.players[id].SendPlayerTeamPacket(id, game.players[id].team_color);

	for (int i = 0; i < MAXPLAYER; ++i) {
		if (game.players[i].state == E_ONLINE && id != i) {
			game.players[id].SendLoginPacket(i);
			game.players[id].SendNamePacket(i, game.players[i].p.name);
			game.players[id].SendPlayerTeamPacket(i, game.players[i].team_color);
		}
	}

	for (int i = 0; i < MAXPLAYER; ++i) {
		if (game.players[i].state == E_ONLINE && id != i) {
			game.players[i].SendLoginPacket(id);
			game.players[i].SendNamePacket(id, game.players[id].p.name);
			game.players[i].SendPlayerTeamPacket(id, game.players[id].team_color);
		}
	}


	printf("%d make thread\n", id);
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
		//ProcessPacket(id, game.players[id].recv_buf);
		while (game.players[id].remain_data > 0)
		{
			BASE_PACKET* bp = reinterpret_cast<BASE_PACKET*>(game.players[id].recv_buf);
			int packetsize = bp->size;
			if (game.players[id].remain_data >= packetsize) {
				ProcessPacket(id, game.players[id].recv_buf);
				game.players[id].remain_data -= packetsize;
				std::memmove(game.players[id].recv_buf, game.players[id].recv_buf + packetsize, game.players[id].remain_data);
			}
			else break;
		}
	}

	if (IsGameEnd()) {
		game.SwitchScene(&game.lobbyScene);

		for (int i = 0; i < MAXPLAYER; ++i) {
			if (game.players[i].state == E_OFFLINE) continue;
			game.players[i].SendScenePacket(id, LOBBY);
		}
	}

	game.players[id].ResetSESSION();
}


void LogicThread()
{
	while (1)
	{
		WaitForSingleObject(event_logic, INFINITE);
		SetEvent(event_event);
		while (game.IsPlayScene()) {
			game.Update();
			Sleep(1);
		}
		ResetEvent(event_logic);
	}
}


void EventThread()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 100);
	std::uniform_int_distribution<int> wind(0, 7);

	std::uniform_int_distribution<int> rect_x(WindowWidth/2 - 300, WindowWidth/2 + 300);
	std::uniform_int_distribution<int> rect_y(WindowHeight/2 - 200, WindowHeight/2 + 200);
	while (1)
	{
		WaitForSingleObject(event_event, INFINITE);
		printf("EventThread On\n");

		while (game.IsPlayScene())
		{
			std::this_thread::sleep_for(std::chrono::seconds(5));
			int num = dis(gen);

			if (num < 33) {		 // 바람 이벤트

				printf("Wind Event On\n");
				int wind_way = wind(gen);
				WindWay(wind_way);

				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(W, 1);
				}
				std::this_thread::sleep_for(std::chrono::seconds(10));

				printf("Wind Event Off\n");
				WindWay(-1);
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(W, 0);
				}
			}

			else if (num < 66) { // 장판 이벤트

				printf("floor Event On\n");
				game.playScene.b_floor = true;
				game.playScene.floor.position.x = rect_x(gen);
				game.playScene.floor.position.y = rect_y(gen);
				game.playScene.floor.size.x = 260;
				game.playScene.floor.size.y = 260;
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(F, 1);
					game.players[i].SendPosPacket(-1, game.playScene.floor.position.x, game.playScene.floor.position.y, E_OBJTYPE::FLOOR);
				}
				std::this_thread::sleep_for(std::chrono::seconds(10));

				printf("floor Event Off\n");
				game.playScene.b_floor = false;
				game.playScene.floor.position.x = 0;
				game.playScene.floor.position.y = 0;
				game.playScene.floor.size.x = 0;
				game.playScene.floor.size.y = 0;
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(F, 0);
				}
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].p.f_friction = 0.0;
				}
				game.playScene.ball.f_friction = 0.0;
			}
			else {				 // 장애물 이벤트 
				printf("obtacle Event On\n");
				game.playScene.b_obtacle = true;
				game.playScene.obstacle.position.x = rect_x(gen);
				game.playScene.obstacle.position.y = rect_y(gen);
				game.playScene.obstacle.size.x = 60;
				game.playScene.obstacle.size.y = 130;
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(O, 1);
					game.players[i].SendPosPacket(-1, game.playScene.obstacle.position.x, game.playScene.obstacle.position.y, E_OBJTYPE::OBSTACLE);
				}				
				std::this_thread::sleep_for(std::chrono::seconds(10));

				printf("obtacle Event Off\n");
				game.playScene.b_obtacle = false;
				game.playScene.obstacle.position.x = 0;
				game.playScene.obstacle.position.y = 0;
				game.playScene.obstacle.size.x = 0;
				game.playScene.obstacle.size.y = 0;
				for (int i = 0; i < MAXPLAYER; ++i) {
					if (game.players[i].state == E_OFFLINE) continue;
					game.players[i].SendEventPacket(O, 0);
				}
			}
		}
		ResetEvent(event_event);
		printf("EventThread Sleep\n");
	}
}

void WindWay(int wind)
{
	for (SESSION& player : game.players) {
		if (player.state == E_OFFLINE) continue;
		if (player.team_color == OBSERVER) continue;
		switch (wind)
		{
		case 0:	// 상
			player.p.wind_velocity.x = 0.0;
			player.p.wind_velocity.y = -10.0;
			break;
		case 1:	// 우상
			player.p.wind_velocity.x = 10.0;
			player.p.wind_velocity.y = -10.0;
			break;
		case 2:	// 우
			player.p.wind_velocity.x = 10.0;
			player.p.wind_velocity.y = 0.0;
			break;
		case 3:	// 우하
			player.p.wind_velocity.x = 10.0;
			player.p.wind_velocity.y = 10.0;
			break;
		case 4:	// 하
			player.p.wind_velocity.x = 0.0;
			player.p.wind_velocity.y = 10.0;
			break;
		case 5:	// 좌하
			player.p.wind_velocity.x = -10.0;
			player.p.wind_velocity.y = 10.0;
			break;
		case 6:	// 좌
			player.p.wind_velocity.x = 10.0;
			player.p.wind_velocity.y = 0.0;
			break;
		case 7:	// 좌상
			player.p.wind_velocity.x = -10.0;
			player.p.wind_velocity.y = -10.0;
			break;
		case -1:// 초기화
			player.p.wind_velocity.x = 0.0;
			player.p.wind_velocity.y = 0.0;
			break;
		default:
			break;
		}
	}
	switch (wind)
	{
	case 0:	// 상
		game.playScene.ball.wind_velocity.x = 0.0;
		game.playScene.ball.wind_velocity.y = -10.0;
		break;
	case 1:	// 우상
		game.playScene.ball.wind_velocity.x = 10.0;
		game.playScene.ball.wind_velocity.y = -10.0;
		break;
	case 2:	// 우
		game.playScene.ball.wind_velocity.x = 10.0;
		game.playScene.ball.wind_velocity.y = 0.0;
		break;
	case 3:	// 우하
		game.playScene.ball.wind_velocity.x = 10.0;
		game.playScene.ball.wind_velocity.y = 10.0;
		break;
	case 4:	// 하
		game.playScene.ball.wind_velocity.x = 0.0;
		game.playScene.ball.wind_velocity.y = 10.0;
		break;
	case 5:	// 좌하
		game.playScene.ball.wind_velocity.x = -10.0;
		game.playScene.ball.wind_velocity.y = 10.0;
		break;
	case 6:	// 좌
		game.playScene.ball.wind_velocity.x = 10.0;
		game.playScene.ball.wind_velocity.y = 0.0;
		break;
	case 7:	// 좌상
		game.playScene.ball.wind_velocity.x = -10.0;
		game.playScene.ball.wind_velocity.y = -10.0;
		break;
	case -1:// 초기화
		game.playScene.ball.wind_velocity.x = 0.0;
		game.playScene.ball.wind_velocity.y = 0.0;
		break;
	default:
		break;
	}
}

int FindRemainID()
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

	printf("red : %d명\tblue : %d명\n", redplayers, blueplayers);

	if (redplayers > blueplayers)
		return BLUE;
	else
		return RED;
}

bool IsGameEnd()
{
	const int online = std::count_if(game.players.begin(), game.players.end(), [](SESSION p) {
		return p.state == E_ONLINE;
		});
	
	if (online == 0)
		return true;
	else
		return false;
}

int main()
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	event_logic = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (event_logic == NULL) err_quit("CreateEvent()");
	event_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (event_event == NULL) err_quit("CreateEvent()");

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
	std::thread p_thread, logic_thread, event_thread;

	logic_thread = std::thread(LogicThread);
	logic_thread.detach();
	event_thread = std::thread(EventThread);
	event_thread.detach();
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
		int id = FindRemainID();
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
	CloseHandle(event_event);
	return 0;
}