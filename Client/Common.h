#pragma once
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h>
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>
#include <chrono>
#include <array>



#define WindowWidth 1440
#define WindowHeight 900
#define ScreenWidth 1280
#define ScreenHeight 720
#define LobbyWidth 1000
#define LobbyHeight 800
#define StartWidth 400
#define StartHeight 200

#define PixelPerMeter 12
#define MeterPerPixel 1/12

#define MAXPLAYER 10

#define WM_SET_FOCUS_TO_PLAY 999
#define WM_SET_FOCUS_TO_LOBBY 998


enum E_team {
	Red,
	Blue,
	Observer,
	Ball,
	Object
};

enum E_item {
	Power,
	Speed,
	Size
};

enum E_STATE { ONLINE, OFFLINE };

enum NETWORK_EVENT {
	SendNone,
	SendStart,
	SendTeamBlue,
	SendTeamRed,
	SendTeamObserver,
	SendSoccer,
	SendBasketball
};

class XY
{
public:
	double x{}, y{};

	XY() {};
	XY(double x, double y) : x(x), y(y) {};
};

class Rect
{
public:
	double left, right, top, bottom;

	Rect() {};
	Rect(double l, double r, double t, double b) :
		left(l), right(r), top(t), bottom(b) {};

	Rect(XY pos, XY size) :
		left(pos.x - size.x / 2), right(pos.x + size.x / 2),
		top(pos.y - size.y / 2), bottom(pos.y + size.y / 2) {};
};
