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



enum E_MAPTYPE { SOCCER, BASKETBALL };
enum E_TEAMCOLOR { RED, BLUE };
enum E_OBJTYPE { BALL, PLAYER };
enum E_SCENEKIND { PLAY, LOBBY };


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
	char key;
};
class START_PACKET : public BASE_PACKET {

};
class POS_PACKET : public BASE_PACKET {
public:
	double x, y;
	E_OBJTYPE objtype;
};
class SCENE_PACKET : public BASE_PACKET {
public:
	E_SCENEKIND scenekind;
};
#pragma pack (pop)