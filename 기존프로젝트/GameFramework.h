#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "Timer.h"



class CGameFramework
{
	CScene* currentScene;
	CTimer timer;

public:
	InputManager* inputManager;
	CPlayScene playScene;
	CLobbyScene lobbyScene;

	CGameFramework() { SwitchScene(&lobbyScene); timer.Start(); };

	void Update();
public:
	//int currentScene = 0;
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
	void SetScene(int i);

	void SwitchScene(CScene* newScene);

	InputManager* getInputManager() { return currentScene->getInputManager(); };
};

