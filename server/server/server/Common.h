#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <iostream>

#define PixelPerMeter 12
#define MeterPerPixel 1/12

#define WindowWidth 1440
#define WindowHeight 900

#define MAXPLAYER  10


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

	Rect() { left = 0, right = 0, top = 0, bottom = 0; };
	Rect(double l, double r, double t, double b) :
		left(l), right(r), top(t), bottom(b) {};

	Rect(XY pos, XY size) :
		left(pos.x - size.x / 2), right(pos.x + size.x / 2),
		top(pos.y - size.y / 2), bottom(pos.y + size.y / 2) {};
};