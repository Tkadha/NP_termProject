#pragma once
#define BUFSIZE    512
constexpr int NAME_SIZE = 20;

constexpr char CS_LOGIN = 0;
constexpr char CS_TEAM_CHOICE = 1;
constexpr char CS_MAP_CHOICE = 2;
constexpr char CS_NAME = 3;
constexpr char CS_KEY = 4;
constexpr char CS_START = 5;
constexpr char CS_KEY_DOWN = 10;



constexpr char SC_LOGIN = 0;
constexpr char SC_TEAM_CHOICE = 1;
constexpr char SC_MAP_CHOICE = 2;
constexpr char SC_NAME = 3;
constexpr char SC_START = 4;




enum E_MAPTYPE { SOCCER, BASKETBALL };
enum E_TEAMCOLOR { RED, BLUE, BALL, OBEJCT };



#pragma pack (push, 1)
class BASE_PACKET {
public:
	unsigned char size;
	char	type;
	int		id;
};

class LOGIN_PACKET : public BASE_PACKET {
public:
};

class NAME_PACKET : public BASE_PACKET {
public:
	char name[NAME_SIZE];
};
class TEAM_PACKET : public BASE_PACKET {
public:
	E_TEAMCOLOR teamcolor;
};
class MAP_PACKET : public BASE_PACKET {
public:
	E_MAPTYPE maptype;
};
class KEY_PACKET : public BASE_PACKET {
public:
	WPARAM key;
};
class START_PACKET : public BASE_PACKET {

};

//--------------------------еб╟Ф-----------------------------
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
#pragma pack (pop)