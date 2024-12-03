#pragma once
#include "Map.h"
#include <math.h>
#include "SESSION.h"
#include <array>

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
	virtual void Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& players) {};

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

	void Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& players);
	
	void Enter(std::array <SESSION, MAXPLAYER>& players) {
		printf("Scene : Lobby\n");
	};
	void Exit(std::array <SESSION, MAXPLAYER>& players) {};
};

class CPlayScene : public CScene
{
private:
	PlayInputManager inputM;

public:
	CPlayScene();

	CBall ball;						// °ø

	CSoccerMap map;

	void ObjectCollisionCheck(std::array <SESSION, MAXPLAYER>& players);
	void Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& players);

	void Reset(std::array <SESSION, MAXPLAYER>& players);

	void Enter(std::array <SESSION, MAXPLAYER>& players);
	void Exit(std::array <SESSION, MAXPLAYER>& players) {};
};



