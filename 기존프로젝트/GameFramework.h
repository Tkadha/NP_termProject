#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "Timer.h"

class CGameFramework
{
	CPlayScene currentScene;

	CTimer timer;

public:
	InputManager inputManager;

	CGameFramework() { timer.Start(); };

	void Update();
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
};

