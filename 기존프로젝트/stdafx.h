#pragma once
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h>
#include <windows.h> //--- ������ ��� ����
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>
#include <chrono>
#include <vector>

#define PixelPerMeter 12
#define MeterPerPixel 1/12

#define ID_RED_BUTTON 1001


enum E_team {
	RedTeam,
	BlueTeam,
	Ball,
	Object
};

enum E_item {
	Power,
	Speed,
	Size
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
