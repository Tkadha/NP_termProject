#pragma once
#include "Map.h"
extern HWND hWnd;
extern HINSTANCE g_hInst;


class CScene
{
public:
	virtual void Enter() {};
	virtual void Update() {};
	virtual void Render(HDC& dc) {};
	virtual void Exit() {};
};


class CStartScene :  public CScene
{
	
};


class CLobbyScene : public CScene
{
private:

	HWND buttonRed, buttonBlue, buttonStart;

public:
	CLobbyScene() {};

	CLobbyMap lobbymap;

	void Render(HDC& dc);
	void Update();

	void Enter() {};
	void Exit();
};

class CPlayScene : public CScene
{
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
};



class InputManager
{
	BOOL KeyDownBuffer[256];
	
public:
	void Update(WPARAM wParam, UINT uMsg);
	BOOL* GetInput();
};

class LobbyInputManager : InputManager
{

};

class PlayInputManager : InputManager
{

};