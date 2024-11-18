#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "Timer.h"



class CGameFramework
{
	HWND hWnd;
	HINSTANCE hInstance;

	CScene* currentScene;
	CTimer timer;

public:
	InputManager inputManager;
	CPlayScene playScene;
	CLobbyScene lobbyScene;

	CGameFramework(HWND hWnd, HINSTANCE hInstance) : hWnd(hWnd), hInstance(hInstance) { 
		SwitchScene(&lobbyScene, hWnd, hInstance);
		timer.Start(); 
	};
	CGameFramework() { timer.Start(); };

	void Update();
public:
	//int currentScene = 0;
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
	void SetScene(int i);

	void SwitchScene(CScene* newScene, HWND hWnd, HINSTANCE hInstance);
};

