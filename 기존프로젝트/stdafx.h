#pragma once
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>

enum E_team {
	RedTeam,
	BlueTeam,
	Ball,
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
