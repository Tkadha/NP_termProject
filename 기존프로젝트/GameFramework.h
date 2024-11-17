#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "Timer.h"



class CGameFramework
{
	CPlayScene playScene;
	CLobbyScene lobbyScene;
	CTimer timer;

public:
	InputManager inputManager;

	CGameFramework() { timer.Start(); };

	void Update();
public:
	int currentScene = 0;
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
	void SetScene(int i);
};

