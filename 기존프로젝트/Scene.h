#pragma once
#include "Map.h"

class CScene
{
public:
	virtual void Enter(HWND hWnd, HINSTANCE hInstance) {};
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
	//CLobbyScene(HWND hWnd, HINSTANCE hInstance) : hWnd(hWnd), hInstance(hInstance) {};

	CLobbyMap lobbymap;

	void Render(HDC& dc);
	void Update();

	void Enter(HWND hWnd, HINSTANCE hInstance);
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

	void Enter(HWND hWnd, HINSTANCE hInstance) {};
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