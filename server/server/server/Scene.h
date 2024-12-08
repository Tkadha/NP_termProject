#pragma once
#include "Map.h"
//#include <math.h>
#include "SESSION.h"
#include <array>
#include "Timer.h"

class InputManager
{
public:
	InputManager() {};
	//InputManager(HWND hwnd, HWND lobby, HWND play) :hwnd(hwnd), lobbwnd(lobby), playwnd(play) {};
	BOOL KeyDownBuffer[256];

	//HWND hwnd, lobbwnd, playwnd;

	virtual void Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
	BOOL* GetInput();
};

class LobbyInputManager : public InputManager
{
public:
	LobbyInputManager() {};
	void Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
};

class PlayInputManager : public InputManager
{
public:
	PlayInputManager() {};
	void Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
};



class CScene
{
public:
	E_MAPTYPE maptype;
	
	virtual void Update(std::array <SESSION, MAXPLAYER>& players) {};

	virtual void Enter(std::array <SESSION, MAXPLAYER>& players) {};
	virtual void Exit(std::array <SESSION, MAXPLAYER>& players) {};
};


class CStartScene : public CScene
{

};


class CLobbyScene : public CScene
{
private:
	LobbyInputManager inputM;

public:
	CLobbyScene() {};

	void Update(std::array <SESSION, MAXPLAYER>& players);
	
	void Enter(std::array <SESSION, MAXPLAYER>& players) {
		printf("Scene : Lobby\n");
	};
	void Exit(std::array <SESSION, MAXPLAYER>& players) {};
};

class CPlayScene : public CScene
{
private:
	PlayInputManager inputM;
	CTimer timer;

	std::chrono::steady_clock::time_point goalTime;
	float goalDuration = 3.0f;
	bool goal = false, kickOff = false;

public:
	CPlayScene();

	CBall ball;						// 공


	CMap* map;
	CSoccerMap soccerMap;
	CBasketballMap basketballMap;
	CRectangleObject obstacle;		// 장애물
	CRectangleObject floor;			// 장판
	bool b_obtacle;
	bool b_floor;

	void ChangeMap(E_MAPTYPE map);
	void ObjectCollisionCheck(std::array <SESSION, MAXPLAYER>& players);
	void Update(std::array <SESSION, MAXPLAYER>& players);

	void Reset(std::array <SESSION, MAXPLAYER>& players);

	void Enter(std::array <SESSION, MAXPLAYER>& players);
	void Exit(std::array <SESSION, MAXPLAYER>& players) {};
};



