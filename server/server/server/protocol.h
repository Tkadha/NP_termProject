#pragma once
#define BUFSIZE    512
constexpr int NAME_SIZE = 20;

constexpr char CS_LOGIN = 1;
constexpr char CS_TEAM_CHOICE = 2;
constexpr char CS_MAP_CHOICE = 3;
constexpr char CS_NAME = 4;
constexpr char CS_KEY = 5;
constexpr char CS_START = 6;
constexpr char CS_EXIT = 7;



constexpr char SC_LOGIN = 1;
constexpr char SC_TEAM_CHOICE = 2;
constexpr char SC_MAP_CHOICE = 3;
constexpr char SC_NAME = 4;
constexpr char SC_START = 5;
constexpr char SC_POS = 6;
constexpr char SC_SCENE = 7;
constexpr char SC_LOGOUT = 8;
constexpr char SC_EVENT = 9;



enum E_MAPTYPE { SOCCER, BASKETBALL };
enum E_TEAMCOLOR { RED, BLUE, OBSERVER };
enum E_OBJTYPE { BALL, PLAYER, OBSTACLE, FLOOR };
enum E_SCENEKIND { PLAY, LOBBY };
enum E_EVENTTYPE { W, F, O };


#pragma pack (push, 1)
class BASE_PACKET {
public:
	unsigned char size;
	char	type;
};

class LOGIN_PACKET : public BASE_PACKET {
public:
	int		id;
};

class NAME_PACKET : public BASE_PACKET {
public:
	int		id;
	char name[NAME_SIZE];
};
class TEAM_PACKET : public BASE_PACKET {
public:
	int		id;
	E_TEAMCOLOR teamcolor;
};
class MAP_PACKET : public BASE_PACKET {
public:
	E_MAPTYPE maptype;
};
class KEY_PACKET : public BASE_PACKET {
public:
	int		id;
	char key;
};
class START_PACKET : public BASE_PACKET {
public:
	int		id;
};
class POS_PACKET : public BASE_PACKET {
public:
	int		id;
	double x, y;
	E_OBJTYPE objtype;
};
class SCENE_PACKET : public BASE_PACKET {
public:
	E_SCENEKIND scenekind;
};
class EVENT_PACKET : public BASE_PACKET {
public:
	E_EVENTTYPE eventtype;
	char onoff;	// 1이면 on 0이면 off
	double x, y;
	int size;
	XY recsize;
};
#pragma pack (pop)