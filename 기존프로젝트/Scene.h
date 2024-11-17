#pragma once
#include "Map.h"

class CScene
{

};


class CStartScene : CScene
{
	
};


class CLobbyScene : CScene
{
public:
	CLobbyMap lobbymap;

	void Render(HDC& dc);
	void Update();

};

class CPlayScene :CScene
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