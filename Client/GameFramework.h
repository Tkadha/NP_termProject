#pragma once
#include "Common.h"
#include "Scene.h"



class CGameFramework
{
	CScene* currentScene;

public:
	InputManager* inputManager;
	CPlayScene playScene;
	CLobbyScene lobbyScene;

	CGameFramework() { SwitchScene(&lobbyScene);};

	void Update();
public:
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);

	void SwitchScene(CScene* newScene);
	void InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg);
};


