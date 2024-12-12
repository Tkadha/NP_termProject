#pragma once
#include "Map.h"
#include "Player.h"

extern HWND hWnd;
extern HINSTANCE g_hInst;

extern HWND lobbyWnd, playWnd;

extern enum E_MAPTYPE;

class InputManager
{
public:
	InputManager() {};
	BOOL KeyDownBuffer[256];

	virtual NETWORK_EVENT Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
	BOOL* GetInput();
};

class LobbyInputManager : public InputManager
{
public:
	LobbyInputManager() {};
	NETWORK_EVENT Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
};

class PlayInputManager : public InputManager
{
public:
	PlayInputManager() {};
	NETWORK_EVENT Update(WPARAM wParam, WPARAM lParam, UINT uMsg);
};



class CScene
{
public:
	E_MAPTYPE maptype{};
	int redScore{}, blueScore{}, goalScore = 3;
	
	virtual ~CScene() {};

	virtual void Enter() {};
	virtual void Update() {};
	virtual void Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players) {};
	virtual void Exit() {};
	virtual void setFocus() {};

	virtual void CircleUpdate(E_team) {};
	
	virtual void SetPos(XY pos) {};
	virtual void SetBallPos(XY pos) {};

	virtual InputManager* getInputManager() { return nullptr; };
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

	CLobbyMap lobbymap;

	void Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players);
	void Update(BOOL KeyDownBuffer[], float timeElapsed);

	void Enter() { };
	void Exit() { };

	void setFocus() { SetFocus(lobbyWnd); };

	InputManager* getInputManager() override { return &inputM; };

	void SetPos(XY pos) {};
};

class CPlayScene : public CScene
{
private:
	PlayInputManager inputM;

public:
	CPlayScene();

	CBall ball;						// °ø


	CMap* map;
	CSoccerMap soccerMap;
	CBasketballMap basketballMap;
	CObstacle obstacle;
	CFloor floor;

	LOGFONT LogFont;
	HFONT hF, oldF;

	void ChangeMap(E_MAPTYPE maptype);

	void Update();
	void Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players);

	void Enter() { ChangeMap(maptype); };
	void Exit() { };

	void setFocus() { PostMessage(hWnd, WM_SET_FOCUS_TO_PLAY, 0, 0); };

	void CircleUpdate(E_team color);

	void SetPos(XY pos);
	void SetBallPos(XY pos);

	InputManager* getInputManager() override { return &inputM; };
};



