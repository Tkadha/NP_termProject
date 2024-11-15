#pragma once
#include "stdafx.h"
#include "Scene.h"



class CGameFramework
{
	CPlayScene playScene;
	CLobbyScene lobbyScene;

public:
	CGameFramework() {};
	int currentScene = 0;
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
	void SetScene(int i);
};

