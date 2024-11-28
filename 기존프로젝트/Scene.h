#pragma once
#include "Map.h"
extern HWND hWnd;
extern HINSTANCE g_hInst;

extern HWND lobbyWnd, playWnd;

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
	virtual void Enter() {};
	virtual void Update(BOOL KeyDownBuffer[], float timeElapsed) {};
	virtual void Render(HDC& dc) {};
	virtual void Exit() {};

	virtual InputManager* getInputManager() { return nullptr; };
};


class CStartScene :  public CScene
{
	
};


class CLobbyScene : public CScene
{
private:
	LobbyInputManager inputM;

public:
	CLobbyScene() {};

	CLobbyMap lobbymap;

	void Render(HDC& dc);
	void Update(BOOL KeyDownBuffer[], float timeElapsed);

	void Enter();
	void Exit();

	InputManager* getInputManager() override { return &inputM; };
};

class CPlayScene : public CScene
{
private:
	PlayInputManager inputM;

public:
	CPlayScene();
	//CSoccerMap soccerMap;

	CPlayer player;					// 나
	CPlayer otherplayers;			// 다른 사람

	CBall ball;						// 공

	CSoccerMap map;

	void ObjectCollisionCheck();
	void Update(BOOL KeyDownBuffer[]);
	void Update(BOOL KeyDownBuffer[], float timeElapsed);
	void Render(HDC& dc);

	void Enter() {};
	void Exit() {};

	InputManager* getInputManager() override { return &inputM; };
};



