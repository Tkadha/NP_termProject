#pragma once
constexpr int NAME_SIZE = 20;

constexpr char CS_TEAM_CHOICE = 1;
constexpr char CS_MAP_CHOICE = 2;




constexpr char SC_TEAM_CHOICE = 1;




enum E_MAPTYPE { SOCCER, BASKETBALL };
enum E_TEAMCOLOR { RED, BLUE };



#pragma pack (push, 1)
class BASE_PACKET {
public:
	unsigned char size;
	char	type;
	int		id;
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
#pragma pack (pop)