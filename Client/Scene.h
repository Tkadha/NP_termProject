#pragma once
#include "Map.h"
#include "Player.h"

extern HWND hWnd;
extern HINSTANCE g_hInst;

extern HWND lobbyWnd, playWnd;

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
	virtual void Enter() {};
	virtual void Update() {};
	virtual void Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players) {};
	virtual void Exit() {};
	virtual void setFocus() {};
	
	// 임시로 만듬
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

	void Enter();
	void Exit();

	void setFocus() { SetFocus(lobbyWnd); };

	InputManager* getInputManager() override { return &inputM; };

	// 임시
	void SetPos(XY pos) {};
};

class CPlayScene : public CScene
{
private:
	PlayInputManager inputM;

public:
	CPlayScene();
	//CSoccerMap soccerMap;

	CBall ball;						// 공

	CSoccerMap map;

	void Update();
	void Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players);

	void Enter() {};
	void Exit() {};

	void setFocus() { PostMessage(hWnd, WM_SET_FOCUS_TO_CHILD, 0, 0); };

	// 임시
	void SetPos(XY pos);
	void SetBallPos(XY pos);

	InputManager* getInputManager() override { return &inputM; };
};



